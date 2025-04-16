/**
 * @file add.rs
 * @author Krisna Pranav
 * @brief add[similar to git add]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs;
use std::path::Path;

pub fn execute(file: String) {
    let path = Path::new(&file);

    if !path.exists() {
        println!("File {} does not exist", file);
        return;
    }

    let staged_file = Path::new(".dvcs/staging").join(path.file_name().unwrap());
    fs::copy(path, staged_file).expect("Failed to add file to staging");

    println!("Added {} to staging area", file);
}
