/**
 * @file log.rs
 * @author Krisna Pranav
 * @brief log[similar to git log]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs;
use std::path::Path;

pub fn execute() {
    let log_path = Path::new(".dvcs/log");

    if !log_path.exists() {
        println!("No commits found.");
        return;
    }

    let log_entries = fs::read_dir(log_path)
        .expect("Failed to read log directory")
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .collect::<Vec<_>>();

    if log_entries.is_empty() {
        println!("No commits found.");
    } else {
        for entry in log_entries {
            println!("Commit: {}", entry.display());
        }
    }
}
