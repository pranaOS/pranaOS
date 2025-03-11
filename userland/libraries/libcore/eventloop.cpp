/**
 * @file eventloop.cpp
 * @author Krisna Pranav
 * @brief event loop
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/badge.h>
#include <mods/debug.h>
#include <mods/format.h>
#include <mods/idallocator.h>
#include <mods/jsonobject.h>
#include <mods/jsonvalue.h>
#include <mods/neverdestroyed.h>
#include <mods/singleton.h>
#include <mods/temporarychange.h>
#include <mods/time.h>
#include <libcore/event.h>
#include <libcore/eventloop.h>
#include <libcore/localserver.h>
#include <libcore/notifier.h>
#include <libcore/object.h>
#include <libthreading/mutex.h>
#include <libthreading/mutexprotected.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#ifdef __pranaos__
extern bool s_global_initializers_ran;
#endif

namespace Core 
{

    class InspectorServerConnection;

    [[maybe_unused]] static bool connect_to_inspector_server();

    struct EventLoopTimer 
    {
        int timer_id { 0 };
        Time interval;
        Time fire_time;
        bool should_reload { false };
        TimerShouldFireWhenNotVisible fire_when_not_visible { TimerShouldFireWhenNotVisible::No };
        WeakPtr<Object> owner;

        /**
         * @param now 
         */
        void reload(Time const& now);

        /**
         * @param now 
         * @return true 
         * @return false 
         */
        bool has_expired(Time const& now) const;
    }; // struct EventLoopTimer 

    struct EventLoop::Private 
    {
        Threading::Mutex lock;
    }; // struct EventLoop::Private 

    static Threading::MutexProtected<NeverDestroyed<IDAllocator>> s_id_allocator;
    static Threading::MutexProtected<RefPtr<InspectorServerConnection>> s_inspector_server_connection;

    static thread_local Vector<EventLoop&>* s_event_loop_stack;
    static thread_local HashMap<int, NonnullOwnPtr<EventLoopTimer>>* s_timers;
    static thread_local HashTable<Notifier*>* s_notifiers;
    thread_local int EventLoop::s_wake_pipe_fds[2];
    thread_local bool EventLoop::s_wake_pipe_initialized { false };
    
    void EventLoop::initialize_wake_pipes()
    {
        if (!s_wake_pipe_initialized) {
    #if defined(SOCK_NONBLOCK)
            int rc = pipe2(s_wake_pipe_fds, O_CLOEXEC);
    #else
            int rc = pipe(s_wake_pipe_fds);
            fcntl(s_wake_pipe_fds[0], F_SETFD, FD_CLOEXEC);
            fcntl(s_wake_pipe_fds[1], F_SETFD, FD_CLOEXEC);

    #endif
            VERIFY(rc == 0);
            s_wake_pipe_initialized = true;
        }
    }

    /**
     * @return true 
     * @return false 
     */
    bool EventLoop::has_been_instantiated()
    {
        return s_event_loop_stack != nullptr && !s_event_loop_stack->is_empty();
    }

    class SignalHandlers : public RefCounted<SignalHandlers> 
    {
        MOD_MAKE_NONCOPYABLE(SignalHandlers);
        MOD_MAKE_NONMOVABLE(SignalHandlers);

    public:
        /**
         * @brief Construct a new Signal Handlers object
         * 
         * @param signo 
         * @param handle_signal 
         */
        SignalHandlers(int signo, void (*handle_signal)(int));

        /**
         * @brief Destroy the SignalHandlers object
         * 
         */
        ~SignalHandlers();

        void dispatch();

        /**
         * @param handler 
         * @return int 
         */
        int add(Function<void(int)>&& handler);

        /**
         * @param handler_id 
         * @return true 
         * @return false 
         */
        bool remove(int handler_id);

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const
        {
            if (m_calling_handlers) {
                for (auto& handler : m_handlers_pending) {
                    if (handler.value)
                        return false; 
                }
            }
            return m_handlers.is_empty();
        }

        /**
         * @param handler_id 
         * @return true 
         * @return false 
         */
        bool have(int handler_id) const
        {
            if (m_calling_handlers) {
                auto it = m_handlers_pending.find(handler_id);
                if (it != m_handlers_pending.end()) {
                    if (!it->value)
                        return false; 
                }
            }
            return m_handlers.contains(handler_id);
        }

        int m_signo;
        void (*m_original_handler)(int); 
        HashMap<int, Function<void(int)>> m_handlers;
        HashMap<int, Function<void(int)>> m_handlers_pending;
        bool m_calling_handlers { false };
    }; // class SignalHandlers : public RefCounted<SignalHandlers> 

    struct SignalHandlersInfo 
    {
        HashMap<int, NonnullRefPtr<SignalHandlers>> signal_handlers;
        int next_signal_id { 0 };
    }; // struct SignalHandlersInfo 

    static Singleton<SignalHandlersInfo> s_signals;

    /**
     * @tparam create_if_null 
     * @return SignalHandlersInfo* 
     */
    template<bool create_if_null = true>
    inline SignalHandlersInfo* signals_info()
    {
        return s_signals.ptr();
    }

    pid_t EventLoop::s_pid;

    class InspectorServerConnection : public Object 
    {
        C_OBJECT(InspectorServerConnection)
    private:
        /**
         * @brief Construct a new InspectorServerConnection object
         * 
         * @param socket 
         */
        explicit InspectorServerConnection(NonnullOwnPtr<Stream::LocalSocket> socket)
            : m_socket(move(socket))
            , m_client_id(s_id_allocator.with_locked([](auto& allocator) {
                return allocator->allocate();
            }))
        {
    #ifdef __pranaos__
            m_socket->on_ready_to_read = [this] {
                u32 length;
                auto maybe_bytes_read = m_socket->read({ (u8*)&length, sizeof(length) });
                if (maybe_bytes_read.is_error()) {
                    dbgln("InspectorServerConnection: Failed to read message length from inspector server connection: {}", maybe_bytes_read.error());
                    shutdown();
                    return;
                }

                auto bytes_read = maybe_bytes_read.release_value();
                if (bytes_read.is_empty()) {
                    dbgln_if(EVENTLOOP_DEBUG, "RPC client disconnected");
                    shutdown();
                    return;
                }

                VERIFY(bytes_read.size() == sizeof(length));

                auto request_buffer = ByteBuffer::create_uninitialized(length).release_value();
                maybe_bytes_read = m_socket->read(request_buffer.bytes());
                if (maybe_bytes_read.is_error()) {
                    dbgln("InspectorServerConnection: Failed to read message content from inspector server connection: {}", maybe_bytes_read.error());
                    shutdown();
                    return;
                }

                bytes_read = maybe_bytes_read.release_value();

                auto request_json = JsonValue::from_string(request_buffer);
                if (request_json.is_error() || !request_json.value().is_object()) {
                    dbgln("RPC client sent invalid request");
                    shutdown();
                    return;
                }

                handle_request(request_json.value().as_object());
            };
    #else
            warnln("RPC Client constructed outside pranaos, this is very likely a bug!");
    #endif
        }

        /**
         * @brief Destroy the Inspector Server Connection object
         * 
         */
        virtual ~InspectorServerConnection() override
        {
            if (auto inspected_object = m_inspected_object.strong_ref())
                inspected_object->decrement_inspector_count({});
        }

    public:
        /**
         * @param response 
         */
        void send_response(JsonObject const& response)
        {
            auto serialized = response.to_string();
            u32 length = serialized.length();

            MUST(m_socket->write({ (u8 const*)&length, sizeof(length) }));
            MUST(m_socket->write(serialized.bytes()));
        }

        /**
         * @param request 
         */
        void handle_request(JsonObject const& request)
        {
            auto type = request.get("type").as_string_or({});

            if (type.is_null()) {
                dbgln("RPC client sent request without type field");
                return;
            }

            if (type == "Identify") {
                JsonObject response;
                response.set("type", type);
                response.set("pid", getpid());
    #ifdef __pranaos__
                char buffer[1024];
                if (get_process_name(buffer, sizeof(buffer)) >= 0) {
                    response.set("process_name", buffer);
                } else {
                    response.set("process_name", JsonValue());
                }
    #endif
                send_response(response);
                return;
            }

            if (type == "GetAllObjects") {
                JsonObject response;
                response.set("type", type);
                JsonArray objects;
                for (auto& object : Object::all_objects()) {
                    JsonObject json_object;
                    object.save_to(json_object);
                    objects.append(move(json_object));
                }
                response.set("objects", move(objects));
                send_response(response);
                return;
            }

            if (type == "SetInspectedObject") {
                auto address = request.get("address").to_number<FlatPtr>();
                for (auto& object : Object::all_objects()) {
                    if ((FlatPtr)&object == address) {
                        if (auto inspected_object = m_inspected_object.strong_ref())
                            inspected_object->decrement_inspector_count({});
                        m_inspected_object = object;
                        object.increment_inspector_count({});
                        break;
                    }
                }
                return;
            }

            if (type == "SetProperty") {
                auto address = request.get("address").to_number<FlatPtr>();
                for (auto& object : Object::all_objects()) {
                    if ((FlatPtr)&object == address) {
                        bool success = object.set_property(request.get("name").to_string(), request.get("value"));
                        JsonObject response;
                        response.set("type", "SetProperty");
                        response.set("success", success);
                        send_response(response);
                        break;
                    }
                }
                return;
            }

            if (type == "Disconnect") {
                shutdown();
                return;
            }
        }

        void shutdown()
        {
            s_id_allocator.with_locked([this](auto& allocator) { allocator->deallocate(m_client_id); });
        }

    private:
        NonnullOwnPtr<Stream::LocalSocket> m_socket;
        WeakPtr<Object> m_inspected_object;
        int m_client_id { -1 };
    }; // class InspectorServerConnection : public Object 

    /**
     * @brief Construct a new EventLoop::EventLoop object
     * 
     * @param make_inspectable 
     */
    EventLoop::EventLoop([[maybe_unused]] MakeInspectable make_inspectable)
        : m_wake_pipe_fds(&s_wake_pipe_fds)
        , m_private(make<Private>())
    {
    #ifdef __pranaos__
        if (!s_global_initializers_ran) {
            VERIFY_NOT_REACHED();
        }
    #endif

        if (!s_event_loop_stack) {
            s_event_loop_stack = new Vector<EventLoop&>;
            s_timers = new HashMap<int, NonnullOwnPtr<EventLoopTimer>>;
            s_notifiers = new HashTable<Notifier*>;
        }

        if (s_event_loop_stack->is_empty()) {
            s_pid = getpid();
            s_event_loop_stack->append(*this);

    #ifdef __pranaos__
            if (getuid() != 0) {
                if (getenv("MAKE_INSPECTABLE") == "1"sv)
                    make_inspectable = Core::EventLoop::MakeInspectable::Yes;

                if (make_inspectable == MakeInspectable::Yes
                    && !s_inspector_server_connection.with_locked([](auto inspector_server_connection) { return inspector_server_connection; })) {
                    if (!connect_to_inspector_server())
                        dbgln("Core::EventLoop: Failed to connect to InspectorServer");
                }
            }
    #endif
        }

        initialize_wake_pipes();

        dbgln_if(EVENTLOOP_DEBUG, "{} Core::EventLoop constructed :)", getpid());
    }

    /**
     * @brief Destroy the EventLoop::EventLoop object
     * 
     */
    EventLoop::~EventLoop()
    {
        if (!s_event_loop_stack->is_empty() && &s_event_loop_stack->last() == this)
            s_event_loop_stack->take_last();
    }

    bool connect_to_inspector_server()
    {
    #ifdef __pranaos__
        auto maybe_socket = Core::Stream::LocalSocket::connect("/tmp/portal/inspectables");
        if (maybe_socket.is_error()) {
            dbgln("connect_to_inspector_server: Failed to connect: {}", maybe_socket.error());
            return false;
        }
        s_inspector_server_connection.with_locked([&](auto& inspector_server_connection) {
            inspector_server_connection = InspectorServerConnection::construct(maybe_socket.release_value());
        });
        return true;
    #else
        VERIFY_NOT_REACHED();
    #endif
    }

    #define VERIFY_EVENT_LOOP_INITIALIZED()                                              \
        do {                                                                             \
            if (!s_event_loop_stack) {                                                   \
                warnln("EventLoop static API was called without prior EventLoop init!"); \
                VERIFY_NOT_REACHED();                                                    \
            }                                                                            \
        } while (0)

    /**
     * @return EventLoop& 
     */
    EventLoop& EventLoop::current()
    {
        VERIFY_EVENT_LOOP_INITIALIZED();
        return s_event_loop_stack->last();
    }

    /**
     * @param code 
     */
    void EventLoop::quit(int code)
    {
        dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop::quit({})", code);
        m_exit_requested = true;
        m_exit_code = code;
    }

    void EventLoop::unquit()
    {
        dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop::unquit()");
        m_exit_requested = false;
        m_exit_code = 0;
    }

    struct EventLoopPusher 
    {
    public:
        /**
         * @brief Construct a new EventLoopPusher object
         * 
         * @param event_loop 
         */
        EventLoopPusher(EventLoop& event_loop)
            : m_event_loop(event_loop)
        {
            if (EventLoop::has_been_instantiated()) {
                m_event_loop.take_pending_events_from(EventLoop::current());
                s_event_loop_stack->append(event_loop);
            }
        }

        /**
         * @brief Destroy the EventLoopPusher object
         * 
         */
        ~EventLoopPusher()
        {
            if (EventLoop::has_been_instantiated()) {
                s_event_loop_stack->take_last();
                EventLoop::current().take_pending_events_from(m_event_loop);
            }
        }

    private:
        EventLoop& m_event_loop;
    }; // struct EventLoopPusher 

    /**
     * @return int 
     */
    int EventLoop::exec()
    {
        EventLoopPusher pusher(*this);
        for (;;) {
            if (m_exit_requested)
                return m_exit_code;
            pump();
        }
        VERIFY_NOT_REACHED();
    }

    /**
     * @param goal_condition 
     */
    void EventLoop::spin_until(Function<bool()> goal_condition)
    {
        EventLoopPusher pusher(*this);
        while (!goal_condition())
            pump();
    }
    
    /**
     * @param mode 
     * @return size_t 
     */
    size_t EventLoop::pump(WaitMode mode)
    {
        wait_for_event(mode);

        decltype(m_queued_events) events;
        {
            Threading::MutexLocker locker(m_private->lock);
            events = move(m_queued_events);
        }

        size_t processed_events = 0;
        for (size_t i = 0; i < events.size(); ++i) {
            auto& queued_event = events.at(i);
            auto receiver = queued_event.receiver.strong_ref();
            auto& event = *queued_event.event;
            if (receiver)
                dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop: {} event {}", *receiver, event.type());

            if (!receiver) {
                switch (event.type()) {
                case Event::Quit:
                    VERIFY_NOT_REACHED();
                default:
                    dbgln_if(EVENTLOOP_DEBUG, "Event type {} with no receiver :(", event.type());
                    break;
                }
            } else if (event.type() == Event::Type::DeferredInvoke) {
                dbgln_if(DEFERRED_INVOKE_DEBUG, "DeferredInvoke: receiver = {}", *receiver);
                static_cast<DeferredInvocationEvent&>(event).m_invokee();
            } else {
                NonnullRefPtr<Object> protector(*receiver);
                receiver->dispatch_event(event);
            }
            ++processed_events;

            if (m_exit_requested) {
                Threading::MutexLocker locker(m_private->lock);
                dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop: Exit requested. Rejigging {} events.", events.size() - i);
                decltype(m_queued_events) new_event_queue;
                new_event_queue.ensure_capacity(m_queued_events.size() + events.size());
                for (++i; i < events.size(); ++i)
                    new_event_queue.unchecked_append(move(events[i]));
                new_event_queue.extend(move(m_queued_events));
                m_queued_events = move(new_event_queue);
                break;
            }
        }

        return processed_events;
    }

    /**
     * @param receiver 
     * @param event 
     * @param should_wake 
     */
    void EventLoop::post_event(Object& receiver, NonnullOwnPtr<Event>&& event, ShouldWake should_wake)
    {
        Threading::MutexLocker lock(m_private->lock);
        dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop::post_event: ({}) << receiver={}, event={}", m_queued_events.size(), receiver, event);
        m_queued_events.empend(receiver, move(event));
        if (should_wake == ShouldWake::Yes)
            wake();
    }

    /**
     * @brief Construct a new SignalHandlers::SignalHandlers object
     * 
     * @param signo 
     * @param handle_signal 
     */
    SignalHandlers::SignalHandlers(int signo, void (*handle_signal)(int))
        : m_signo(signo)
        , m_original_handler(signal(signo, handle_signal))
    {
        dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop: Registered handler for signal {}", m_signo);
    }

    /**
     * @brief Destroy the SignalHandlers::SignalHandlers object
     * 
     */
    SignalHandlers::~SignalHandlers()
    {
        dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop: Unregistering handler for signal {}", m_signo);
        signal(m_signo, m_original_handler);
    }

    void SignalHandlers::dispatch()
    {
        TemporaryChange change(m_calling_handlers, true);
        for (auto& handler : m_handlers)
            handler.value(m_signo);
        if (!m_handlers_pending.is_empty()) {
            for (auto& handler : m_handlers_pending) {
                if (handler.value) {
                    auto result = m_handlers.set(handler.key, move(handler.value));
                    VERIFY(result == Mods::HashSetResult::InsertedNewEntry);
                } else {
                    m_handlers.remove(handler.key);
                }
            }
            m_handlers_pending.clear();
        }
    }   

    /**
     * @param handler 
     * @return int 
     */
    int SignalHandlers::add(Function<void(int)>&& handler)
    {
        int id = ++signals_info()->next_signal_id; 

        if (m_calling_handlers)
            m_handlers_pending.set(id, move(handler));
        else
            m_handlers.set(id, move(handler));
        return id;
    }

    /**
     * @param handler_id 
     * @return true 
     * @return false 
     */
    bool SignalHandlers::remove(int handler_id)
    {
        VERIFY(handler_id != 0);

        if (m_calling_handlers) {
            auto it = m_handlers.find(handler_id);
            if (it != m_handlers.end()) {
                m_handlers_pending.set(handler_id, {});
                return true;
            }

            it = m_handlers_pending.find(handler_id);

            if (it != m_handlers_pending.end()) {
                if (!it->value)
                    return false; 
                it->value = nullptr;
                return true;
            }

            return false;
        }

        return m_handlers.remove(handler_id);
    }

    /**
     * @param signo 
     */
    void EventLoop::dispatch_signal(int signo)
    {
        auto& info = *signals_info();
        auto handlers = info.signal_handlers.find(signo);
        if (handlers != info.signal_handlers.end()) {
            auto handler = handlers->value;
            dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop: dispatching signal {}", signo);
            handler->dispatch();
        }
    }

    /**
     * @param signo 
     */
    void EventLoop::handle_signal(int signo)
    {
        VERIFY(signo != 0);

        if (getpid() == s_pid) {
            int nwritten = write(s_wake_pipe_fds[1], &signo, sizeof(signo));
            if (nwritten < 0) {
                perror("EventLoop::register_signal: write");
                VERIFY_NOT_REACHED();
            }
        } else {
            s_pid = 0;
        }
    }

    /**
     * @param signo 
     * @param handler 
     * @return int 
     */
    int EventLoop::register_signal(int signo, Function<void(int)> handler)
    {
        VERIFY(signo != 0);
        auto& info = *signals_info();
        auto handlers = info.signal_handlers.find(signo);

        if (handlers == info.signal_handlers.end()) {
            auto signal_handlers = adopt_ref(*new SignalHandlers(signo, EventLoop::handle_signal));
            auto handler_id = signal_handlers->add(move(handler));
            info.signal_handlers.set(signo, move(signal_handlers));
            return handler_id;
        } else {
            return handlers->value->add(move(handler));
        }
    }

    /**
     * @param handler_id 
     */
    void EventLoop::unregister_signal(int handler_id)
    {
        VERIFY(handler_id != 0);
        int remove_signo = 0;
        auto& info = *signals_info();

        for (auto& h : info.signal_handlers) {
            auto& handlers = *h.value;
            if (handlers.remove(handler_id)) {
                if (handlers.is_empty())
                    remove_signo = handlers.m_signo;
                break;
            }
        }

        if (remove_signo != 0)
            info.signal_handlers.remove(remove_signo);
    }

    /**
     * @param event 
     */
    void EventLoop::notify_forked(ForkEvent event)
    {
        VERIFY_EVENT_LOOP_INITIALIZED();

        switch (event) {
        case ForkEvent::Child:
            s_event_loop_stack->clear();
            s_timers->clear();
            s_notifiers->clear();
            s_wake_pipe_initialized = false;
            initialize_wake_pipes();
            if (auto* info = signals_info<false>()) {
                info->signal_handlers.clear();
                info->next_signal_id = 0;
            }
            s_pid = 0;
            return;
        }

        VERIFY_NOT_REACHED();
    }

    /**
     * @param mode 
     */
    void EventLoop::wait_for_event(WaitMode mode)
    {
        fd_set rfds;
        fd_set wfds;
    retry:
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);

        int max_fd = 0;
        auto add_fd_to_set = [&max_fd](int fd, fd_set& set) {
            FD_SET(fd, &set);
            if (fd > max_fd)
                max_fd = fd;
        };

        int max_fd_added = -1;
        add_fd_to_set(s_wake_pipe_fds[0], rfds);
        max_fd = max(max_fd, max_fd_added);

        for (auto& notifier : *s_notifiers) {
            if (notifier->event_mask() & Notifier::Read)
                add_fd_to_set(notifier->fd(), rfds);
            if (notifier->event_mask() & Notifier::Write)
                add_fd_to_set(notifier->fd(), wfds);
            if (notifier->event_mask() & Notifier::Exceptional)
                VERIFY_NOT_REACHED();
        }

        bool queued_events_is_empty;
        {
            Threading::MutexLocker locker(m_private->lock);
            queued_events_is_empty = m_queued_events.is_empty();
        }

        Time now;
        struct timeval timeout = { 0, 0 };
        bool should_wait_forever = false;
        if (mode == WaitMode::WaitForEvents && queued_events_is_empty) {
            auto next_timer_expiration = get_next_timer_expiration();
            if (next_timer_expiration.has_value()) {
                now = Time::now_monotonic_coarse();
                auto computed_timeout = next_timer_expiration.value() - now;
                if (computed_timeout.is_negative())
                    computed_timeout = Time::zero();
                timeout = computed_timeout.to_timeval();
            } else {
                should_wait_forever = true;
            }
        }

    try_select_again:
        int marked_fd_count = select(max_fd + 1, &rfds, &wfds, nullptr, should_wait_forever ? nullptr : &timeout);
        if (marked_fd_count < 0) {
            int saved_errno = errno;
            if (saved_errno == EINTR) {
                if (m_exit_requested)
                    return;
                goto try_select_again;
            }
            dbgln("Core::EventLoop::wait_for_event: {} ({}: {})", marked_fd_count, saved_errno, strerror(saved_errno));
            VERIFY_NOT_REACHED();
        }
        if (FD_ISSET(s_wake_pipe_fds[0], &rfds)) {
            int wake_events[8];
            ssize_t nread;

            do {
                errno = 0;
                nread = read(s_wake_pipe_fds[0], wake_events, sizeof(wake_events));
                if (nread == 0)
                    break;
            } while (nread < 0 && errno == EINTR);
            if (nread < 0) {
                perror("Core::EventLoop::wait_for_event: read from wake pipe");
                VERIFY_NOT_REACHED();
            }
            VERIFY(nread > 0);
            bool wake_requested = false;
            int event_count = nread / sizeof(wake_events[0]);
            for (int i = 0; i < event_count; i++) {
                if (wake_events[i] != 0)
                    dispatch_signal(wake_events[i]);
                else
                    wake_requested = true;
            }

            if (!wake_requested && nread == sizeof(wake_events))
                goto retry;
        }

        if (!s_timers->is_empty()) {
            now = Time::now_monotonic_coarse();
        }

        for (auto& it : *s_timers) {
            auto& timer = *it.value;
            if (!timer.has_expired(now))
                continue;
            auto owner = timer.owner.strong_ref();
            if (timer.fire_when_not_visible == TimerShouldFireWhenNotVisible::No
                && owner && !owner->is_visible_for_timer_purposes()) {
                continue;
            }

            dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop: Timer {} has expired, sending Core::TimerEvent to {}", timer.timer_id, *owner);

            if (owner)
                post_event(*owner, make<TimerEvent>(timer.timer_id));
            if (timer.should_reload) {
                timer.reload(now);
            } else {
                VERIFY_NOT_REACHED();
            }
        }

        if (!marked_fd_count)
            return;

        for (auto& notifier : *s_notifiers) {
            if (FD_ISSET(notifier->fd(), &rfds)) {
                if (notifier->event_mask() & Notifier::Event::Read)
                    post_event(*notifier, make<NotifierReadEvent>(notifier->fd()));
            }
            if (FD_ISSET(notifier->fd(), &wfds)) {
                if (notifier->event_mask() & Notifier::Event::Write)
                    post_event(*notifier, make<NotifierWriteEvent>(notifier->fd()));
            }
        }
    }

    /**
     * @param now 
     * @return true 
     * @return false 
     */
    bool EventLoopTimer::has_expired(Time const& now) const
    {
        return now > fire_time;
    }

    /**
     * @param now 
     */
    void EventLoopTimer::reload(Time const& now)
    {
        fire_time = now + interval;
    }

    /**
     * @return Optional<Time> 
     */
    Optional<Time> EventLoop::get_next_timer_expiration()
    {
        auto now = Time::now_monotonic_coarse();
        Optional<Time> soonest {};

        for (auto& it : *s_timers) {
            auto& fire_time = it.value->fire_time;
            auto owner = it.value->owner.strong_ref();
            if (it.value->fire_when_not_visible == TimerShouldFireWhenNotVisible::No
                && owner && !owner->is_visible_for_timer_purposes()) {
                continue;
            }

            if (fire_time < now)
                return now;
            if (!soonest.has_value() || fire_time < soonest.value())
                soonest = fire_time;
        }
        return soonest;
    }

    /**
     * @param object 
     * @param milliseconds 
     * @param should_reload 
     * @param fire_when_not_visible 
     * @return int 
     */
    int EventLoop::register_timer(Object& object, int milliseconds, bool should_reload, TimerShouldFireWhenNotVisible fire_when_not_visible)
    {
        VERIFY_EVENT_LOOP_INITIALIZED();
        VERIFY(milliseconds >= 0);

        auto timer = make<EventLoopTimer>();

        timer->owner = object;
        timer->interval = Time::from_milliseconds(milliseconds);
        timer->reload(Time::now_monotonic_coarse());
        timer->should_reload = should_reload;
        timer->fire_when_not_visible = fire_when_not_visible;

        int timer_id = s_id_allocator.with_locked([](auto& allocator) { return allocator->allocate(); });

        timer->timer_id = timer_id;
        s_timers->set(timer_id, move(timer));

        return timer_id;
    }

    /**
     * @param timer_id 
     * @return true 
     * @return false 
     */
    bool EventLoop::unregister_timer(int timer_id)
    {
        VERIFY_EVENT_LOOP_INITIALIZED();
        s_id_allocator.with_locked([&](auto& allocator) { allocator->deallocate(timer_id); });
        auto it = s_timers->find(timer_id);

        if (it == s_timers->end())
            return false;

        s_timers->remove(it);
        return true;
    }

    /**
     * @param notifier 
     */
    void EventLoop::register_notifier(Badge<Notifier>, Notifier& notifier)
    {
        VERIFY_EVENT_LOOP_INITIALIZED();
        s_notifiers->set(&notifier);
    }

    /**
     * @param notifier 
     */
    void EventLoop::unregister_notifier(Badge<Notifier>, Notifier& notifier)
    {
        VERIFY_EVENT_LOOP_INITIALIZED();
        s_notifiers->remove(&notifier);
    }

    void EventLoop::wake_current()
    {
        EventLoop::current().wake();
    }

    void EventLoop::wake()
    {
        dbgln_if(EVENTLOOP_DEBUG, "Core::EventLoop::wake()");

        int wake_event = 0;
        int nwritten = write((*m_wake_pipe_fds)[1], &wake_event, sizeof(wake_event));

        if (nwritten < 0) {
            perror("EventLoop::wake: write");
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @brief Construct a new EventLoop::QueuedEvent::QueuedEvent object
     * 
     * @param receiver 
     * @param event 
     */
    EventLoop::QueuedEvent::QueuedEvent(Object& receiver, NonnullOwnPtr<Event> event)
        : receiver(receiver)
        , event(move(event))
    {
    }

    /**
     * @brief Construct a new EventLoop::QueuedEvent::QueuedEvent object
     * 
     * @param other 
     */
    EventLoop::QueuedEvent::QueuedEvent(QueuedEvent&& other)
        : receiver(other.receiver)
        , event(move(other.event))
    {
    }

} // namespace Core
