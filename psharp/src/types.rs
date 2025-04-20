/**
 * @file types.rs
 * @author Krisna Pranav
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use std::ops::Range;

pub type Spanned<T> = (T, Span);
pub type Span = Range<usize>;