use std::time::Duration;
use downcast_rs::{impl_downcast, Downcast};
/**
 * @file lib.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-05-14
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

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
    repeat: bool
}

impl Resource for TimerWrap {}

struct TaskWrap {
}