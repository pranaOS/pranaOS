/**
 * @file status.rs
 * @author Krisna Pranav
 * @brief status[similar to git status]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs;

pub fn execute() {
    let staged_path = ".dvcs/staging";

    if !std::path::Path::new(staged_path).exists() {
        println!("No files are staged.");
        return;
    }

    let staged_files = fs::read_dir(staged_path)
        .expect("Failed to read staging directory")
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .collect::<Vec<_>>();

    if staged_files.is_empty() {
        println!("No files are staged.");
    } else {
        println!("Staged files:");
        for file in staged_files {
            println!("{}", file.display());
        }
    }
}
