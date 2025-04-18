/**
 * @file: errors.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::{error::Error, fmt::Display};

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum BrushError {
    TooBigCacheTexture(u32),
}

impl Error for BrushError {}

impl Display for BrushError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "wgpu-text: ")?;
        match self {
            BrushError::TooBigCacheTexture(cache_texture) => {
                
            }
        }
    }
}