/**
 * @file blob.rs
 * @author Krisna Pranav
 * @brief blob
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use crate::utils;
use anyhow::Result;
use std::fs;

pub struct Blob {
    pub content: Vec<u8>,
}

impl Blob {
    pub fn new(data: Vec<u8>) -> Self {
        Self { content: data }
    }

    pub fn hash(&self) -> String {
        utils::hash_data(&self.content)
    }

    pub fn serialize(&self) -> Result<Vec<u8>> {
        utils::compress(&self.content)
    }

    pub fn store(&self) -> Result<String> {
        let hash = self.hash();
        let path = utils::object_path(&hash);
        fs::create_dir_all(path.parent().unwrap())?;
        fs::write(path, self.serialize()?)?;
        Ok(hash)
    }
}
