/**
 * @file init.rs
 * @author Krisna Pranav
 * @brief init[similar to git init]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs;
use std::path::Path;

pub fn execute(path: String) {
    let repo_path = Path::new(&path);

    if repo_path.exists() {
        println!("Repository already exists at {}", path);
        return;
    }

    fs::create_dir_all(repo_path.join(".dvcs")).expect("Failed to initialize repository");

    println!("Repository initialized at {}", path);
}
