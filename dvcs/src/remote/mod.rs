/**
 * @file mod.rs
 * @author Krisna Pranav
 * @brief mod
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs::{create_dir_all, write, read_to_string};
use std::path::Path;

pub mod http;

const REMOTE_FILE: &str = ".dvcs/remotes/origin";

pub fn set_remote(url: &str) -> std::io::Result<()> {
    let dir = Path::new(".dvcs/remotes");
    create_dir_all(dir)?;
    write(REMOTE_FILE, url)?;
    Ok(())
}

pub fn get_remote() -> std::io::Result<String> {
    Ok(read_to_string(REMOTE_FILE)?)
}
