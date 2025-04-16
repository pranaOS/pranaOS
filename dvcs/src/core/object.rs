/**
 * @file object.rs
 * @author Krisna Pranav
 * @brief object
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#[derive(Debug)]
pub enum ObjectType {
    Blob,
    Tree,
    Commit,
    Tag,
}
