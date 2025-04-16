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

pub fn package_repository() -> anyhow::Result<Vec<u8>> {
    let mut data = Vec::new();

    for entry in fs::read_dir(".dvcs/objects")? {
        if let Ok(entry) = entry {
            let file_data = fs::read(entry.path())?;
            data.extend(file_data);
        }
    }

    Ok(data)
}


pub fn apply_remote_data(data: Vec<u8>) -> anyhow::Result<()> {
    use std::fs::write;
    write(".dvcs/remote_pack", data)?;
    println!("Fetched data saved to .dvcs/remote_pack");
    Ok(())
}

