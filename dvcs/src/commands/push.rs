/**
 * @file push.rs
 * @author Krisna Pranav
 * @brief push [similar to git push]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use crate::networking::http::push_to_remote;

pub fn execute(remote: String) {
    if let Err(e) = push_to_remote(remote) {
        println!("Failed to push to remote: {}", e);
    } else {
        println!("Successfully pushed changes to remote");
    }
}
