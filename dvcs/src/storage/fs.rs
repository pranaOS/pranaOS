/**
 * @file fs.rs
 * @author Krisna Pranav
 * @brief fs
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::fs::{self, File};
use std::path::Path;
use std::io::{self, Write};
use crate::objects::commit::Commit;

pub fn save_commit(commit: Commit) -> io::Result<()> {
    let path = Path::new(".dvcs/commits");
    if !path.exists() {
        fs::create_dir_all(path)?;
    }

    let commit_path = path.join(path); 
    let mut file = File::create(commit_path)?;
    file.write(&commit.serialize()?.into_bytes())?;

    Ok(())
}
