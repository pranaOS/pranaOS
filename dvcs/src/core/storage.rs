/**
 * @file storage.rs
 * @author Krisna Pranav
 * @brief storage
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs;
use std::path::Path;

pub fn write_object(hash: &str, data: &[u8]) -> anyhow::Result<()> {
    let path = Path::new(".dvcs/objects").join(hash);
    fs::write(path, data)?;
    Ok(())
}
