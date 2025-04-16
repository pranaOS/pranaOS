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

use crate::core::{hash, storage};
use std::fs;
use std::path::Path;

pub fn run(path: &str) -> anyhow::Result<()> {
    let data = fs::read(path)?;
    let hash = hash::hash_blob(&data);

    storage::write_object(&hash, &data)?;

    println!("Added '{}' to staging area with hash {}", path, hash);

    Ok(())
}
