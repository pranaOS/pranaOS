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

pub fn run() -> anyhow::Result<()> {
    let head_path = Path::new(".dvcs/HEAD");

    if !head_path.exists() {
        println!("No commits found.");
        return Ok(());
    }

    let hash = fs::read_to_string(head_path)?;
    let object_path = format!(".dvcs/objects/{}", hash.trim());

    if let Ok(commit) = fs::read_to_string(&object_path) {
        println!("Latest commit:\n{}", commit);
    } else {
        println!("No commit object found for hash.");
    }

    Ok(())
}
