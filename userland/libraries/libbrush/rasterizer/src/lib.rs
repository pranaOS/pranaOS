/**
 * @file lib.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#[cfg(not(feature = "std"))]
#[macro_use]
extern crate alloc;

#[cfg(all(feature = "libm", not(feature = "std")))]
mod nostd_float;

#[cfg(not(any(feature = "libm", feature = "std")))]
compile_error!("You need to activate either the `std` or `libm` feature.");

mod geometry;
mod raster;

pub use geometry::{point, Point};
pub use raster::Rasterizer;