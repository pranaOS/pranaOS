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

pub fn run() -> anyhow::Result<()> {
    let dvcs_dir = Path::new(".dvcs");

    if dvcs_dir.exists() {
        println!("Repository already initialized.");
    } else {
        fs::create_dir_all(dvcs_dir.join("objects"))?;
        fs::create_dir_all(dvcs_dir.join("refs"))?;
        fs::write(dvcs_dir.join("HEAD"), b"ref: refs/heads/master\n")?;

        println!("Initialized empty DVCS repository.");
    }

    Ok(())
}
