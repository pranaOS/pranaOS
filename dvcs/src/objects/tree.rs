/**
 * @file tree.rs
 * @author Krisna Pranav
 * @brief tree
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use serde::{Serialize, Deserialize};
use crate::utils;

#[derive(Serialize, Deserialize)]
pub struct TreeEntry {
    pub mode: String,
    pub name: String,
    pub hash: String,
}

#[derive(Serialize, Deserialize)]
pub struct Tree {
    pub entries: Vec<TreeEntry>,
}

impl Tree {
    pub fn new(entries: Vec<TreeEntry>) -> Self {
        Self { entries }
    }

    pub fn hash(&self) -> String {
        let encoded = serde_json::to_vec(&self).unwrap();
        utils::hash_data(&encoded)
    }

    pub fn serialize(&self) -> anyhow::Result<Vec<u8>> {
        let encoded = serde_json::to_vec(&self)?;
        utils::compress(&encoded)
    }
}
