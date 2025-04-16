/**
 * @file commit.rs
 * @author Krisna Pranav
 * @brief blob
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use serde::{Serialize, Deserialize};
use crate::utils;

#[derive(Serialize, Deserialize)]
pub struct Commit {
    pub tree: String,
    pub message: String,
}

impl Commit {
    pub fn new(tree: String, message: String) -> Self {
        Self { tree, message }
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
