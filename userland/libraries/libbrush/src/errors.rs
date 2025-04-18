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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BrushError {
    TooBigCacheTexture(u32),
}

impl Error for BrushError {}

impl Display for BrushError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "wgpu-text: ")?;
        match self {
            BrushError::TooBigCacheTexture(dimensions) => write!(
                f,
                "While trying to resize the \
                cache texture, the 'wgpu::Limits {{ max_texture_dimension_2d }}' \
                limit of {} was crossed!\n\
                Resizing the cache texture should be avoided \
                from the start by building TextBrush with \
                BrushBuilder::initial_cache_size() and providing bigger cache \
                texture dimensions.",
                dimensions
            ),
        }
    }
}