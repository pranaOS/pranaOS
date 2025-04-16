/**
 * @file pull.rs
 * @author Krisna Pranav
 * @brief pull [similar to git pull]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use crate::networking::http::pull_from_remote;

pub fn execute(remote: String) {
    if let Err(e) = pull_from_remote(remote) {
        println!("Failed to pull from remote: {}", e);
    } else {
        println!("Successfully pulled changes from remote");
    }
}
