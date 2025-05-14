/**
 * @file lib.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-05-14
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use anyhow::anyhow;
use anyhow::bail;
use anyhow::Result;
use downcast_rs::impl_downcast;
use downcast_rs::Downcast;
use mio::net::TcpListener;
use mio::net::TcpStream;
use mio::Events;
use mio::Interest;
use mio::Poll;
use mio::Registry;
use mio::Token;
use mio::Waker;
pub use notify::Event as FsEvent;
pub use notify::EventKind as FsEventKind;
use notify::RecommendedWatcher;
use notify::RecursiveMode;
use notify::Watcher;
use rayon::ThreadPool;
use rayon::ThreadPoolBuilder;
pub use signal_hook::consts::signal as Signal;
use signal_hook::low_level::emulate_default_handler;
use std::cell::Cell;
use std::collections::BTreeMap;
use std::collections::HashMap;
use std::collections::LinkedList;
use std::io;
use std::io::Read;
use std::io::Write;
use std::net::Shutdown;
use std::net::SocketAddr;
use std::num::NonZeroUsize;
use std::path::Path;
use std::path::PathBuf;
use std::rc::Rc;
use std::sync::mpsc;
use std::sync::Arc;
use std::sync::Mutex;
use std::thread;
use std::time::Duration;
use std::time::Instant;

#[cfg(target_family = "unix")]
use signal_hook_mio::v0_8::Signals;

pub type Index = u32;

trait Resource: Downcast + 'static {
    fn close(&mut self) {}
}

impl_downcast!(Resource);

struct TimerWrap {
    cb: Box<dyn FnMut(LoopHandle) + 'static>,
    expires_at: Duration,
    repeat: bool,
}

impl Resource for TimerWrap {}

struct TaskWrap {
    inner: Option<TaskOnFinish>,
}

impl Resource for TaskWrap {}

type OneShotCallback = dyn FnOnce(LoopHandle) + 'static;

type Task = Box<dyn FnOnce() -> TaskResult + Send>;
type TaskOnFinish = Box<dyn FnOnce(LoopHandle, TaskResult) + 'static>;
pub type TaskResult = Option<Result<Vec<u8>>>;

type TcpOnConnection = Box<dyn FnOnce(LoopHandle, Index, Result<TcpSocketInfo>) + 'static>;
type TcpListenerOnConnection = Box<dyn FnMut(LoopHandle, Index, Result<TcpSocketInfo>) + 'static>;
type TcpOnWrite = Box<dyn FnOnce(LoopHandle, Index, Result<usize>) + 'static>;
type TcpOnRead = Box<dyn FnMut(LoopHandle, Index, Result<Vec<u8>>) + 'static>;

type FsWatchOnEvent = Box<dyn FnMut(LoopHandle, FsEvent) + 'static>;

struct TcpStreamWrap {
    id: Index,
    socket: TcpStream,
    on_connection: Option<TcpOnConnection>,
    on_read: Option<TcpOnRead>,
    write_queue: LinkedList<(Vec<u8>, TcpOnWrite)>,
}

impl Resource for TcpStreamWrap {
    #[allow(unused_must_use)]
    fn close(&mut self) {
        self.socket.shutdown(Shutdown::Write);
    }
}

struct TcpListenerWrap {
    id: Index,
    socket: TcpListener,
    on_connection: TcpListenerOnConnection,
}

impl Resource for TcpListenerWrap {}

#[allow(dead_code)]
pub struct TcpSocketInfo {
    pub id: Index,
    pub host: SocketAddr,
    pub remote: SocketAddr,
}

struct CheckWrap {
    cb: Option<Box<OneShotCallback>>,
}

impl Resource for CheckWrap {}

struct FsWatcherWrap {
    inner: Option<RecommendedWatcher>,
    on_event: Option<FsWatchOnEvent>,
    path: PathBuf,
    recursive: bool,
}

impl Resource for FsWatcherWrap {}

const MAX_SIGNAL_VALUE: i32 = 31;

#[derive(Debug)]
struct SignalNum(i32);

impl SignalNum {
    fn parse(signal: i32) -> Result<Self> {
        if signal > MAX_SIGNAL_VALUE {
            bail!("Signal number out of range.");
        }

        if signal_hook::consts::FORBIDDEN.contains(&signal) {
            bail!("Forbidden signal provided.");
        }

        Ok(SignalNum(signal))
    }

    fn to_i32(&self) -> i32 {
        self.0
    }
}

struct SignalWrap {
    id: Index,
    on_signal: SignalHandler,
    oneshot: bool,
}

type SignalHandler = Box<dyn FnMut(LoopHandle, i32) + 'static>;

struct OsSignals {
    #[cfg(target_family = "unix")]
    sources: Signals,
    handlers: HashMap<i32, Vec<SignalWrap>>,
}

impl OsSignals {
    #[cfg(target_family = "unix")]
    fn new(registry: &Registry) -> Self {
        let mut sources = Signals::new::<[_; 0], i32>([]).unwrap();
        let _ = registry.register(&mut sources, Token(1), Interest::READABLE);

        OsSignals {
            sources,
            handlers: HashMap::new(),
        }
    }

    #[cfg(target_family = "windows")]
    fn new(notifier: Arc<Mutex<mpsc::Sender<Event>>>, waker: Arc<Waker>) -> Self {
        let on_signal_handler = move || {
            notifier.lock().unwrap().send(Event::WinSigInt).unwrap();
            waker.wake().unwrap();
        };

        ctrlc::set_handler(on_signal_handler).unwrap();

        OsSignals {
            handlers: HashMap::new(),
        }
    }

    #[cfg(target_family = "unix")]
    fn run_pending(&mut self, handle: LoopHandle) {
        for signal in self.sources.pending() {
            let handlers = match self.handlers.get_mut(&signal) {
                Some(handlers) => handlers,
                None => continue,
            };

            if handlers.is_empty() {
                emulate_default_handler(signal).unwrap();
                continue;
            }

            handlers.retain_mut(|handler| {
                (handler.on_signal)(handle.clone(), signal);
                !handler.oneshot
            });
        }
    }

    #[cfg(target_family = "windows")]
    fn run_pending(&mut self, handle: LoopHandle) {
        let handlers = match self.handlers.get_mut(&Signal::SIGINT) {
            Some(handlers) if !handlers.is_empty() => handlers,
            _ => {
                emulate_default_handler(Signal::SIGINT).unwrap();
                return;
            }
        };

        handlers.retain_mut(|handler| {
            (handler.on_signal)(handle.clone(), Signal::SIGINT);
            !handler.oneshot
        });
    }

    fn remove_handler(&mut self, index: Index) {
        if let Some((_, list)) = self
            .handlers
            .iter_mut()
            .find(|(_, list)| list.iter().any(|handler| handler.id == index))
        {
            list.retain(|handler| handler.id != index);
        }
    }
}

#[allow(clippy::enum_variant_names)]
enum Action {
    TimerReq(Index, TimerWrap),
    TimerRemoveReq(Index),
    SpawnReq(Index, Task, TaskWrap),
    TcpConnectionReq(Index, TcpStreamWrap),
    TcpListenReq(Index, TcpListenerWrap),
    TcpWriteReq(Index, Vec<u8>, TcpOnWrite),
    TcpReadStartReq(Index, TcpOnRead),
    TcpCloseReq(Index, Box<OneShotCallback>),
    TcpShutdownReq(Index, Box<OneShotCallback>),
    CheckReq(Index, CheckWrap),
    CheckRemoveReq(Index),
    FsEventStartReq(Index, FsWatcherWrap),
    FsEventStopReq(Index),
    SignalStartReq(SignalNum, SignalWrap),
    SignalStopReq(Index),
}

#[allow(dead_code)]
enum Event {
    ThreadPool(Index, TaskResult),
    Network(TcpEvent),
    Watch(Index, FsEvent),
    WinSigInt,
}

#[derive(Debug)]
enum TcpEvent {
    Read(Index),
    Write(Index),
}

struct FsEventHandler {
    id: Index,
    waker: Arc<Waker>,
    sender: Arc<Mutex<mpsc::Sender<Event>>>,
}

impl notify::EventHandler for FsEventHandler {
    fn handle_event(&mut self, event: notify::Result<FsEvent>) {
        let event = Event::Watch(self.id, event.unwrap());

        self.sender.lock().unwrap().send(event).unwrap();
        self.waker.wake().unwrap();
    }
}

pub struct EventLoop {
    index: Rc<Cell<Index>>,
    resources: HashMap<Index, Box<dyn Resource>>,
    timer_queue: BTreeMap<Instant, Index>,
    action_queue: mpsc::Receiver<Action>,
    action_queue_empty: Rc<Cell<bool>>,
    action_dispatcher: Rc<mpsc::Sender<Action>>,
    check_queue: Vec<Index>,
    close_queue: Vec<(Index, Option<Box<OneShotCallback>>)>,
    thread_pool: ThreadPool,
    thread_pool_tasks: usize,
    event_dispatcher: Arc<Mutex<mpsc::Sender<Event>>>,
    event_queue: mpsc::Receiver<Event>,
    network_events: Registry,
    signals: OsSignals,
    poll: Poll,
    waker: Arc<Waker>,
}

impl EventLoop {
    pub fn new(num_threads: usize) -> Self {
        assert!(num_threads > 0);

        let (action_dispatcher, action_queue) = mpsc::channel();
        let (event_dispatcher, event_queue) = mpsc::channel();

        let thread_pool = ThreadPoolBuilder::new()
            .num_threads(num_threads)
            .build()
            .unwrap();

        let event_dispatcher = Arc::new(Mutex::new(event_dispatcher));

        let poll = Poll::new().unwrap();
        let registry = poll.registry().try_clone().unwrap();
        let waker = Waker::new(poll.registry(), Token(0)).unwrap();
        let waker = Arc::new(waker);

        #[cfg(target_family = "windows")]
        let signals = OsSignals::new(event_dispatcher.clone(), waker.clone());

        #[cfg(target_family = "unix")]
        let signals = OsSignals::new(&registry);

        EventLoop {
            index: Rc::new(Cell::new(2)),
            resources: HashMap::new(),
            timer_queue: BTreeMap::new(),
            action_queue,
            action_queue_empty: Rc::new(Cell::new(true)),
            action_dispatcher: Rc::new(action_dispatcher),
            check_queue: Vec::new(),
            close_queue: Vec::new(),
            thread_pool,
            thread_pool_tasks: 0,
            event_dispatcher,
            event_queue,
            poll,
            network_events: registry,
            waker,
            signals,
        }
    }

    pub fn handle(&self) -> LoopHandle {
        LoopHandle {
            index: self.index.clone(),
            actions: self.action_dispatcher.clone(),
            actions_queue_empty: self.action_queue_empty.clone(),
        }
    }

    pub fn interrupt_handle(&self) -> LoopInterruptHandle {
        LoopInterruptHandle {
            waker: self.waker.clone(),
        }
    }

    pub fn has_pending_events(&self) -> bool {
        !(self.resources.is_empty() && self.action_queue_empty.get() && self.thread_pool_tasks == 0)
    }

    pub fn tick(&mut self) {
        self.prepare();
        self.run_timers();
        self.run_poll();
        self.run_check();
        self.run_close();
    }
}

impl EventLoop {
    fn prepare(&mut self) {
        while let Ok(action) = self.action_queue.try_recv() {
            match action {
                Action::TimerReq(index, timer) => self.timer_req(index, timer),
                Action::TimerRemoveReq(index) => self.timer_remove_req(index),
                Action::SpawnReq(index, task, t_wrap) => self.spawn_req(index, task, t_wrap),
                Action::TcpConnectionReq(index, tc_wrap) => self.tcp_connection_req(index, tc_wrap),
                Action::TcpListenReq(index, tc_wrap) => self.tcp_listen_req(index, tc_wrap),
                Action::TcpWriteReq(index, data, cb) => self.tcp_write_req(index, data, cb),
                Action::TcpReadStartReq(index, cb) => self.tcp_read_start_req(index, cb),
                Action::TcpCloseReq(index, cb) => self.tcp_close_req(index, cb),
                Action::TcpShutdownReq(index, cb) => self.tcp_shutdown_req(index, cb),
                Action::CheckReq(index, cb) => self.check_req(index, cb),
                Action::CheckRemoveReq(index) => self.check_remove_req(index),
                Action::FsEventStartReq(index, w_wrap) => self.fs_event_start_req(index, w_wrap),
                Action::FsEventStopReq(index) => self.fs_event_stop_req(index),
                Action::SignalStartReq(signum, s_wrap) => self.signal_start_req(signum, s_wrap),
                Action::SignalStopReq(index) => self.signal_stop_req(index),
            };
        }
        self.action_queue_empty.set(true);
    }

    fn run_timers(&mut self) {
        let timers_to_remove: Vec<Instant> = self
            .timer_queue
            .range(..Instant::now())
            .map(|(k, _)| *k)
            .collect();

        let indexes: Vec<Index> = timers_to_remove
            .iter()
            .filter_map(|instant| self.timer_queue.remove(instant))
            .collect();

        indexes.iter().for_each(|index| {
            let handle = self.handle();

            if let Some(timer) = self
                .resources
                .get_mut(index)
                .map(|resource| resource.downcast_mut::<TimerWrap>().unwrap())
            {
                (timer.cb)(handle);

                if timer.repeat {
                    let time_key = Instant::now() + timer.expires_at;
                    self.timer_queue.insert(time_key, *index);
                } else {
                    self.resources.remove(index);
                }
            }
        });

        self.prepare();
    }

    fn run_poll(&mut self) {
        let timeout = if self.has_pending_events() {
            let refs = self.check_queue.len() + self.close_queue.len();
            match self.timer_queue.iter().next() {
                _ if refs > 0 => Some(Duration::ZERO),
                Some((t, _)) => Some(*t - Instant::now()),
                None => None,
            }
        } else {
            Some(Duration::ZERO)
        };

        let mut events = Events::with_capacity(1024);

        if let Err(e) = self.poll.poll(&mut events, timeout) {
            match e.kind() {
                io::ErrorKind::Interrupted => return,
                _ => panic!("{}", e),
            };
        }

        for event in &events {
            if event.token() == Token(0) {
                continue;
            }

            if event.token() == Token(1) {
                self.signals.run_pending(self.handle());
                continue;
            }

            let event_type = match (
                event.is_readable() || event.is_read_closed(),
                event.is_writable(),
            ) {
                (true, false) => TcpEvent::Read(event.token().0 as u32),
                (false, true) => TcpEvent::Write(event.token().0 as u32),
                _ => continue,
            };

            self.event_dispatcher
                .lock()
                .unwrap()
                .send(Event::Network(event_type))
                .unwrap();
        }

        while let Ok(event) = self.event_queue.try_recv() {
            match event {
                Event::ThreadPool(index, result) => self.task_complete(index, result),
                Event::Watch(index, event) => self.fs_event(index, event),
                Event::Network(tcp_event) => match tcp_event {
                    TcpEvent::Write(index) => self.tcp_socket_write(index),
                    TcpEvent::Read(index) => self.tcp_socket_read(index),
                },
                Event::WinSigInt => self.signals.run_pending(self.handle()),
            }
            self.prepare();
        }
    }

    fn run_check(&mut self) {
        let handle = self.handle();

        for rid in self.check_queue.drain(..) {
            let mut resource = match self.resources.remove(&rid) {
                Some(resource) => resource,
                None => continue,
            };

            if let Some(cb) = resource
                .downcast_mut::<CheckWrap>()
                .map(|wrap| wrap.cb.take().unwrap())
            {
                (cb)(handle.clone());
            }
        }
        self.prepare();
    }

    fn run_close(&mut self) {
        let handle = self.handle();

        for (rid, on_close) in self.close_queue.drain(..) {
            if let Some(mut resource) = self.resources.remove(&rid) {
                resource.close();
                if let Some(cb) = on_close {
                    (cb)(handle.clone());
                }
            }
        }
        self.prepare();
    }
}

impl EventLoop {
    fn task_complete(&mut self, index: Index, result: TaskResult) {
        if let Some(mut resource) = self.resources.remove(&index) {
            let task_wrap = resource.downcast_mut::<TaskWrap>().unwrap();
            let callback = task_wrap.inner.take().unwrap();
            (callback)(self.handle(), result);
        }
        self.thread_pool_tasks -= 1;
    }

    fn tcp_socket_write(&mut self, index: Index) {
        let handle = self.handle();

        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        let tcp_wrap = resource.downcast_mut::<TcpStreamWrap>().unwrap();

        if let Ok(Some(e)) | Err(e) = tcp_wrap.socket.take_error() {
            if let Some(on_connection) = tcp_wrap.on_connection.take() {
                (on_connection)(handle, index, Result::Err(e.into()));
                return;
            }

            if let Some((_, on_write)) = tcp_wrap.write_queue.pop_front() {
                (on_write)(handle, index, Result::Err(e.into()));
                return;
            }
        }

        if let Some(on_connection) = tcp_wrap.on_connection.take() {
            (on_connection)(
                handle.clone(),
                index,
                Ok(TcpSocketInfo {
                    id: index,
                    host: tcp_wrap.socket.local_addr().unwrap(),
                    remote: tcp_wrap.socket.peer_addr().unwrap(),
                }),
            );

            let token = Token(index as usize);

            self.network_events
                .reregister(&mut tcp_wrap.socket, token, Interest::READABLE)
                .unwrap();
        }

        loop {
            let handle = handle.clone();

            let (data, on_write) = match tcp_wrap.write_queue.pop_front() {
                Some(value) => value,
                None => break,
            };

            match tcp_wrap.socket.write(&data) {
                Ok(n) if n < data.len() => {
                    let err_message = io::ErrorKind::WriteZero.to_string();
                    (on_write)(handle, index, Result::Err(anyhow!("{}", err_message)));
                }

                Ok(n) => (on_write)(handle, index, Result::Ok(n)),

                Err(e) if e.kind() == io::ErrorKind::WouldBlock => {
                    tcp_wrap.write_queue.push_front((data, on_write));
                    break;
                }
                Err(e) if e.kind() == io::ErrorKind::Interrupted => continue,

                Err(e) => (on_write)(handle, index, Result::Err(e.into())),
            };
        }

        if tcp_wrap.write_queue.is_empty() {
            let token = Token(tcp_wrap.id as usize);
            self.network_events
                .reregister(&mut tcp_wrap.socket, token, Interest::READABLE)
                .unwrap();
        }
    }

    fn tcp_socket_read(&mut self, index: Index) {
        let handle = self.handle();

        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        if resource.downcast_ref::<TcpListenerWrap>().is_some() {
            self.tcp_try_accept(index);
            return;
        }

        let tcp_wrap = resource.downcast_mut::<TcpStreamWrap>().unwrap();

        let mut data = vec![];
        let mut data_buf = [0; 4096];

        let mut read_error: Option<io::Error> = None;
        let mut connection_closed = false;

        loop {
            match tcp_wrap.socket.read(&mut data_buf) {
                Ok(0) => {
                    connection_closed = true;
                    break;
                }
                Ok(n) => data.extend_from_slice(&data_buf[..n]),

                Err(e) if e.kind() == io::ErrorKind::WouldBlock => break,
                Err(e) if e.kind() == io::ErrorKind::Interrupted => continue,
                Err(e) => read_error = Some(e),
            }
        }

        let on_read = match tcp_wrap.on_read.as_mut() {
            Some(on_read) => on_read,
            None if !connection_closed => return,
            None => {
                self.network_events
                    .deregister(&mut tcp_wrap.socket)
                    .unwrap();

                self.close_queue.push((index, None));
                return;
            }
        };

        if let Some(e) = read_error {
            (on_read)(handle, index, Result::Err(e.into()));
            return;
        }

        match data.len() {
            0 => (on_read)(handle, index, Result::Ok(data)),
            _ if !connection_closed => (on_read)(handle, index, Result::Ok(data)),
            _ => {
                (on_read)(handle.clone(), index, Result::Ok(data));
                (on_read)(handle, index, Result::Ok(vec![]));
            }
        };
    }

    fn tcp_try_accept(&mut self, index: Index) {
        let handle = self.handle();

        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        let tcp_wrap = match resource.downcast_mut::<TcpListenerWrap>() {
            Some(tcp_wrap) => tcp_wrap,
            None => return,
        };

        let on_connection = tcp_wrap.on_connection.as_mut();
        let mut new_resources = vec![];

        loop {
            let handle = handle.clone();

            let (socket, _) = match tcp_wrap.socket.accept() {
                Ok(sock) => sock,

                Err(e) if e.kind() == io::ErrorKind::WouldBlock => break,
                Err(e) => {
                    (on_connection)(handle, index, Result::Err(e.into()));
                    break;
                }
            };

            let id = handle.index();

            let mut stream = TcpStreamWrap {
                id,
                socket,
                on_connection: None,
                on_read: None,
                write_queue: LinkedList::new(),
            };

            (on_connection)(
                handle,
                id,
                Ok(TcpSocketInfo {
                    id,
                    host: stream.socket.local_addr().unwrap(),
                    remote: stream.socket.peer_addr().unwrap(),
                }),
            );

            self.network_events
                .register(&mut stream.socket, Token(id as usize), Interest::READABLE)
                .unwrap();

            new_resources.push((id, Box::new(stream)));
        }

        for (id, stream) in new_resources.drain(..) {
            self.resources.insert(id, stream);
        }
    }

    fn fs_event(&mut self, index: Index, event: FsEvent) {
        let handle = self.handle();
        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        let on_event = match resource.downcast_mut::<FsWatcherWrap>() {
            Some(w_wrap) => w_wrap.on_event.as_mut().unwrap(),
            None => return,
        };

        (on_event)(handle, event);
    }
}

impl EventLoop {
    fn timer_req(&mut self, index: Index, timer: TimerWrap) {
        let time_key = Instant::now() + timer.expires_at;
        self.resources.insert(index, Box::new(timer));
        self.timer_queue.insert(time_key, index);
    }

    fn timer_remove_req(&mut self, index: Index) {
        self.resources.remove(&index);
        self.timer_queue.retain(|_, v| *v != index);
    }

    fn spawn_req(
        &mut self,
        index: Index,
        task: Box<dyn FnOnce() -> TaskResult + Send>,
        task_wrap: TaskWrap,
    ) {
        let notifier = self.event_dispatcher.clone();

        if task_wrap.inner.is_some() {
            self.resources.insert(index, Box::new(task_wrap));
        }

        self.thread_pool.spawn({
            let waker = self.waker.clone();
            move || {
                let result = (task)();
                let notifier = notifier.lock().unwrap();

                notifier.send(Event::ThreadPool(index, result)).unwrap();
                waker.wake().unwrap();
            }
        });

        self.thread_pool_tasks += 1;
    }

    fn tcp_connection_req(&mut self, index: Index, mut tcp_wrap: TcpStreamWrap) {
        let socket = &mut tcp_wrap.socket;
        let token = Token(tcp_wrap.id as usize);

        self.network_events
            .register(socket, token, Interest::WRITABLE)
            .unwrap();

        self.resources.insert(index, Box::new(tcp_wrap));
    }

    fn tcp_listen_req(&mut self, index: Index, mut tcp_wrap: TcpListenerWrap) {
        let listener = &mut tcp_wrap.socket;
        let token = Token(tcp_wrap.id as usize);

        self.network_events
            .register(listener, token, Interest::READABLE)
            .unwrap();

        self.resources.insert(index, Box::new(tcp_wrap));
    }

    fn tcp_write_req(&mut self, index: Index, data: Vec<u8>, on_write: TcpOnWrite) {
        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        let tcp_wrap = resource.downcast_mut::<TcpStreamWrap>().unwrap();
        let token = Token(index as usize);

        tcp_wrap.write_queue.push_back((data, on_write));

        let interest = Interest::READABLE.add(Interest::WRITABLE);

        self.network_events
            .reregister(&mut tcp_wrap.socket, token, interest)
            .unwrap();
    }

    fn tcp_read_start_req(&mut self, index: Index, on_read: TcpOnRead) {
        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        let tcp_wrap = resource.downcast_mut::<TcpStreamWrap>().unwrap();
        let token = Token(index as usize);

        tcp_wrap.on_read = Some(on_read);

        let interest = match tcp_wrap.write_queue.len() {
            0 => Interest::READABLE,
            _ => Interest::READABLE.add(Interest::WRITABLE),
        };

        self.network_events
            .reregister(&mut tcp_wrap.socket, token, interest)
            .unwrap();
    }

    fn tcp_close_req(&mut self, index: Index, on_close: Box<dyn FnOnce(LoopHandle) + 'static>) {
        self.close_queue.push((index, Some(on_close)));
    }

    fn tcp_shutdown_req(
        &mut self,
        index: Index,
        on_shutdown: Box<dyn FnOnce(LoopHandle) + 'static>,
    ) {
        let resource = match self.resources.get_mut(&index) {
            Some(resource) => resource,
            None => return,
        };

        resource.downcast_mut::<TcpStreamWrap>().unwrap().close();
        on_shutdown(self.handle())
    }

    fn check_req(&mut self, index: Index, check_wrap: CheckWrap) {
        self.resources.insert(index, Box::new(check_wrap));
        self.check_queue.push(index);
    }

    fn check_remove_req(&mut self, index: Index) {
        self.resources.remove(&index);
        self.check_queue.retain(|v| *v != index);
    }

    fn fs_event_start_req(&mut self, index: Index, mut wrap: FsWatcherWrap) {
        let mut watcher = RecommendedWatcher::new(
            FsEventHandler {
                waker: self.waker.clone(),
                sender: self.event_dispatcher.clone(),
                id: index,
            },
            notify::Config::default(),
        )
            .unwrap();

        let recursive_mode = match wrap.recursive {
            true => RecursiveMode::Recursive,
            _ => RecursiveMode::NonRecursive,
        };

        watcher.watch(&wrap.path, recursive_mode).unwrap();

        wrap.inner = Some(watcher);
        self.resources.insert(index, Box::new(wrap));
    }

    fn fs_event_stop_req(&mut self, index: Index) {
        self.resources.remove(&index);
    }

    fn signal_start_req(&mut self, signum: SignalNum, signal_wrap: SignalWrap) {
        let signum = signum.to_i32();
        if let Some(handlers) = self.signals.handlers.get_mut(&signum) {
            handlers.push(signal_wrap);
            return;
        }

        #[cfg(target_family = "unix")]
        {
            self.signals.sources.add_signal(signum).unwrap();
        }
        self.signals.handlers.insert(signum, vec![signal_wrap]);
    }

    fn signal_stop_req(&mut self, index: Index) {
        self.signals.remove_handler(index)
    }
}

impl Default for EventLoop {
    fn default() -> Self {
        let default_pool_size = unsafe { NonZeroUsize::new_unchecked(4) };
        let num_cores = thread::available_parallelism().unwrap_or(default_pool_size);

        Self::new(num_cores.into())
    }
}

#[derive(Clone)]
pub struct LoopHandle {
    index: Rc<Cell<Index>>,
    actions: Rc<mpsc::Sender<Action>>,
    actions_queue_empty: Rc<Cell<bool>>,
}

#[allow(dead_code)]
impl LoopHandle {
    pub fn index(&self) -> Index {
        let index = self.index.get();
        self.index.set(index + 1);
        index
    }

    pub fn timer<F>(&self, delay: u64, repeat: bool, cb: F) -> Index
    where
        F: FnMut(LoopHandle) + 'static,
    {
        let index = self.index();
        let expires_at = Duration::from_millis(delay);

        let timer = TimerWrap {
            cb: Box::new(cb),
            expires_at,
            repeat,
        };

        self.actions.send(Action::TimerReq(index, timer)).unwrap();
        self.actions_queue_empty.set(false);

        index
    }

    pub fn remove_timer(&self, index: &Index) {
        self.actions.send(Action::TimerRemoveReq(*index)).unwrap();
        self.actions_queue_empty.set(false);
    }

    pub fn spawn<F, U>(&self, task: F, task_cb: Option<U>) -> Index
    where
        F: FnOnce() -> TaskResult + Send + 'static,
        U: FnOnce(LoopHandle, TaskResult) + 'static,
    {
        let index = self.index();

        let task_cb: Option<Box<dyn FnOnce(LoopHandle, TaskResult)>> = match task_cb {
            Some(cb) => Some(Box::new(cb)),
            None => None,
        };

        let task_wrap = TaskWrap { inner: task_cb };

        self.actions
            .send(Action::SpawnReq(index, Box::new(task), task_wrap))
            .unwrap();

        self.actions_queue_empty.set(false);

        index
    }

    pub fn tcp_connect<F>(&self, address: &str, on_connection: F) -> Result<Index>
    where
        F: FnOnce(LoopHandle, Index, Result<TcpSocketInfo>) + 'static,
    {
        let address: SocketAddr = address.parse()?;
        let index = self.index();

        let socket = TcpStream::connect(address)?;

        let stream = TcpStreamWrap {
            id: index,
            socket,
            on_connection: Some(Box::new(on_connection)),
            on_read: None,
            write_queue: LinkedList::new(),
        };

        self.actions
            .send(Action::TcpConnectionReq(index, stream))
            .unwrap();

        self.actions_queue_empty.set(false);

        Ok(index)
    }

    pub fn tcp_listen<F>(&self, host: &str, on_connection: F) -> Result<Index>
    where
        F: FnMut(LoopHandle, Index, Result<TcpSocketInfo>) + 'static,
    {
        let address: SocketAddr = host.parse()?;
        let index = self.index();

        let socket = TcpListener::bind(address)?;

        let listener = TcpListenerWrap {
            id: index,
            socket,
            on_connection: Box::new(on_connection),
        };

        self.actions
            .send(Action::TcpListenReq(index, listener))
            .unwrap();

        self.actions_queue_empty.set(false);

        Ok(index)
    }

    pub fn tcp_write<F>(&self, index: Index, data: &[u8], on_write: F)
    where
        F: FnOnce(LoopHandle, Index, Result<usize>) + 'static,
    {
        self.actions
            .send(Action::TcpWriteReq(
                index,
                data.to_vec(),
                Box::new(on_write),
            ))
            .unwrap();

        self.actions_queue_empty.set(false);
    }

    pub fn tcp_read_start<F>(&self, index: Index, on_read: F)
    where
        F: FnMut(LoopHandle, Index, Result<Vec<u8>>) + 'static,
    {
        self.actions
            .send(Action::TcpReadStartReq(index, Box::new(on_read)))
            .unwrap();

        self.actions_queue_empty.set(false);
    }

    pub fn tcp_close<F>(&self, index: Index, on_close: F)
    where
        F: FnOnce(LoopHandle) + 'static,
    {
        self.actions
            .send(Action::TcpCloseReq(index, Box::new(on_close)))
            .unwrap();

        self.actions_queue_empty.set(false);
    }

    pub fn tcp_shutdown<F>(&self, index: Index, on_shutdown: F)
    where
        F: FnOnce(LoopHandle) + 'static,
    {
        self.actions
            .send(Action::TcpShutdownReq(index, Box::new(on_shutdown)))
            .unwrap();

        self.actions_queue_empty.set(false);
    }

    pub fn check<F>(&self, on_check: F) -> Index
    where
        F: FnOnce(LoopHandle) + 'static,
    {
        let index = self.index();
        let on_check = Box::new(on_check);

        self.actions
            .send(Action::CheckReq(index, CheckWrap { cb: Some(on_check) }))
            .unwrap();

        self.actions_queue_empty.set(false);

        index
    }

    pub fn remove_check(&self, index: &Index) {
        self.actions.send(Action::CheckRemoveReq(*index)).unwrap();
        self.actions_queue_empty.set(false);
    }

    pub fn fs_event_start<F, P>(&self, path: P, recursive: bool, on_event: F) -> Result<Index>
    where
        F: FnMut(LoopHandle, FsEvent) + 'static,
        P: AsRef<Path>,
    {
        let index = self.index();
        let on_event = Box::new(on_event);

        std::fs::metadata(path.as_ref())?;

        let watcher_wrap = FsWatcherWrap {
            inner: None,
            on_event: Some(on_event),
            path: path.as_ref().to_path_buf(),
            recursive,
        };

        self.actions
            .send(Action::FsEventStartReq(index, watcher_wrap))
            .unwrap();

        self.actions_queue_empty.set(false);

        Ok(index)
    }

    pub fn fs_event_stop(&self, index: &Index) {
        self.actions.send(Action::FsEventStopReq(*index)).unwrap();
        self.actions_queue_empty.set(false);
    }

    fn signal_init<F>(&self, signum: i32, oneshot: bool, on_signal: F) -> Result<Index>
    where
        F: FnMut(LoopHandle, i32) + 'static,
    {
        let signum = SignalNum::parse(signum)?;

        let index = self.index();
        let on_signal = Box::new(on_signal);

        let signal_wrap = SignalWrap {
            id: index,
            oneshot,
            on_signal,
        };

        self.actions
            .send(Action::SignalStartReq(signum, signal_wrap))
            .unwrap();

        self.actions_queue_empty.set(false);

        Ok(index)
    }

    pub fn signal_start<F>(&self, signum: i32, on_signal: F) -> Result<Index>
    where
        F: FnMut(LoopHandle, i32) + 'static,
    {
        self.signal_init(signum, false, on_signal)
    }

    pub fn signal_start_oneshot<F>(&self, signum: i32, on_signal: F) -> Result<Index>
    where
        F: FnMut(LoopHandle, i32) + 'static,
    {
        self.signal_init(signum, true, on_signal)
    }

    pub fn signal_stop(&self, index: &Index) {
        self.actions.send(Action::SignalStopReq(*index)).unwrap();
        self.actions_queue_empty.set(false);
    }
}

#[derive(Clone)]
pub struct LoopInterruptHandle {
    waker: Arc<Waker>,
}

impl LoopInterruptHandle {
    pub fn interrupt(&self) {
        self.waker.wake().unwrap();
    }
}