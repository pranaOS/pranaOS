/**
 * @file lib.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

mod errors;

pub use glyph_brush;

pub type Matrix = [[f32; 4]; 4];

#[rustfmt::skip]
pub fn ortho(width: f32, height: f32) -> Matrix {
    [
        [2.0 / width, 0.0,          0.0, 0.0],
        [0.0,        -2.0 / height, 0.0, 0.0],
        [0.0,         0.0,          1.0, 0.0],
        [-1.0,        1.0,          0.0, 1.0]
    ]
}