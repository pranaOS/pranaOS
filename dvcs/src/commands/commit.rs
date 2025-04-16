/**
 * @file commit.rs
 * @author Krisna Pranav
 * @brief commit[similar to git commit]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs;
use std::time::{SystemTime, UNIX_EPOCH};
use crate::core::{hash, storage};

pub fn run(message: &str) -> anyhow::Result<()> {
    let timestamp = SystemTime::now().duration_since(UNIX_EPOCH)?.as_secs();
    let commit = format!("commit {}\n{}", timestamp, message);
    let commit_hash = hash::hash_blob(commit.as_bytes());

    storage::write_object(&commit_hash, commit.as_bytes())?;
    fs::write(".dvcs/HEAD", commit_hash.as_bytes())?;

    println!("Committed as {}", commit_hash);
    
    Ok(())
}
