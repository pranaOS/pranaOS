/**
 * @file lib.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2024-05-2
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use ansi_escapes::{CursorMove, CursorTo};
use colored::Colorize;
use image::{DynamicImage, GenericImageView, Pixel, Rgba};

pub struct Image {
    pub path: String,
    image: Option<DynamicImage>,
    cached_image: Option<(u16, u16, Vec<String>)>,
}

impl Image {
    pub fn new(path: &str) -> Image {
        Image {
            path: path.to_owned(),
            image: None,
            cached_image: None,
        }
    }

    fn generated_cache_image(&self, width: u16, height: u16, image: &DynamicImage) -> (u16, u16, Vec<String>) {
        let pixels: Vec<_> = image.pixels().collect();
        let (image_width, image_height) = image.dimensions();
        return Image::transform_pixels_to_lines(width, height, &pixels, image_width, image_height);
    }

    fn transform_pixels_to_lines(width: u16, height: u16, pixels: &[(u32, u32, Rgba<u8>)], image_width: u32, image_height: u32) {

    }
}
