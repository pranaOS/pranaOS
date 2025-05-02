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
use image::{DynamicImage, GenericImageView, Pixel, Rgba};
use colored::Colorize;

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

    fn generate_cached_image(&self, width: u16, height: u16, image: &DynamicImage) -> (u16, u16, Vec<String>) {
        let pixels: Vec<_> = image.pixels().collect();
        let (image_width, image_height) = image.dimensions();
        return Image::transform_pixels_to_lines(width, height, &pixels, image_width, image_height);
    }

    fn transform_pixels_to_lines(width: u16, height: u16, pixels: &[(u32, u32, Rgba<u8>)], image_width: u32, image_height: u32) -> (u16, u16, Vec<String>) {
        let mut lines = Vec::new();

        for y in 0..height {
            let mut line = String::new();
            for x in 0..width {
                let r_x = (x as f32 / width as f32 * image_width as f32) as u32;
                let r_y = (y as f32 / height as f32 * image_height as f32) as u32;
                let r_half_y =
                    (r_y + ((y + 1) as f32 / height as f32 * image_height as f32) as u32) / 2;
                let top_pixel = pixels[(r_y * image_width + r_x) as usize];
                let bottom_pixel = pixels[(r_half_y * image_width + r_x) as usize];
                let top_color = top_pixel.2 .0;
                let bottom_color = bottom_pixel.2 .0;
                line.push_str(&format!(
                    "{}",
                    "▄"
                        .truecolor(bottom_color[0], bottom_color[1], bottom_color[2])
                        .on_truecolor(top_color[0], top_color[1], top_color[2])
                ));
            }
            lines.push(line);
        }

        (width, height, lines)
    }

    fn lazy_load(&mut self, width: u16, height: u16) {
        if let None = self.image {
            self.image = Some(image::open(&self.path).unwrap());
        }

        if let Some(im) = &self.image {
            if let None = self.cached_image {
                self.cached_image = Some(self.generate_cached_image(width, height, im));
            }
            if let Some(c) = self.cached_image.as_ref() {
                if c.0 != width && c.1 != height {
                    self.cached_image = Some(self.generate_cached_image(width, height, im));
                }
            }
        } else {
            panic!("No image");
        }
    }

    pub fn render_at_position(&mut self, x: u16, y: u16, width: u16, height: u16) {
        self.lazy_load(width, height);

        let image = self.cached_image.as_ref().expect("No cached image");
        print!("{}", CursorTo::AbsoluteXY(x, y));
        let char_rows = &image.2;

        for char_row in char_rows {
            print!("{}{}", char_row, CursorMove::XY(-(width as i16), 1))
        }
    }

    pub fn render(&mut self, width: u16, height: u16) {
        self.lazy_load(width, height);
        let image = self.cached_image.as_ref().expect("No cached image");
        let char_rows = &image.2;

        for char_row in char_rows {
            println!("{}", char_row);
        }
    }

    pub fn render_pixels_at_position(x: u16, y: u16, width: u16, height: u16, pixels: &[[u8; 4]], pixel_width: usize) {
        print!("{}", CursorTo::AbsoluteXY(x, y));

        let numbered_pixels: Vec<(u32, u32, Rgba<u8>)> = pixels
            .iter()
            .enumerate()
            .map(|(i, pixel)| {
                let x = i % pixel_width;
                let y = i / pixel_width;
                let color = Rgba::<u8>::from_slice(pixel);
                (x as u32, y as u32, color.clone())
            })
            .collect();

        let char_rows = Image::transform_pixels_to_lines(
            width,
            height,
            &numbered_pixels,
            pixel_width as u32,
            pixels.len() as u32 / pixel_width as u32,
        );

        let char_rows = char_rows.2;

        for char_row in char_rows {
            print!("{}{}", char_row, CursorMove::XY(-(width as i16), 1))
        }
    }

    pub fn render_pixels(width: u16, height: u16, pixels: &[[u8; 4]], pixel_width: usize) {
        let numbered_pixels: Vec<(u32, u32, Rgba<u8>)> = pixels
            .iter()
            .enumerate()
            .map(|(i, pixel)| {
                let x = i % pixel_width;
                let y = i / pixel_width;
                let color = Rgba::<u8>::from_slice(pixel);
                (x as u32, y as u32, color.clone())
            })
            .collect();

        let char_rows = Image::transform_pixels_to_lines(
            width,
            height,
            &numbered_pixels,
            pixel_width as u32,
            pixels.len() as u32 / pixel_width as u32,
        );

        let char_rows = char_rows.2;

        for char_row in char_rows {
            print!("{}{}", char_row, CursorMove::XY(-(width as i16), 1))
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use image::Rgba;

    #[test]
    fn test_transform_pixels_to_lines_basic() {
        let width = 2;
        let height = 2;
        let image_width = 2;
        let image_height = 2;

        let pixels: Vec<(u32, u32, Rgba<u8>)> = vec![
            (0, 0, Rgba([255, 0, 0, 255])),
            (1, 0, Rgba([0, 255, 0, 255])),
            (0, 1, Rgba([0, 0, 255, 255])),
            (1, 1, Rgba([255, 255, 0, 255])),
        ];

        let (_w, _h, lines) = Image::transform_pixels_to_lines(width, height, &pixels, image_width, image_height);

        assert_eq!(lines.len(), 2);
        for line in lines {
            assert!(line.contains("▄"));
        }
    }
}
