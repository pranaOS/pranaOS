/**
 * @file utils.rs
 * @author Krisna Pranav
 * @brief utils
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use blake3::Hasher;
use flate2::{Compression, write::ZlibEncoder, read::ZlibDecoder};
use std::io::{Read, Write};
use std::path::PathBuf;

pub fn hash_data(data: &[u8]) -> String {
    let mut hasher = Hasher::new();
    hasher.update(data);
    hasher.finalize().to_hex().to_string()
}

pub fn compress(data: &[u8]) -> anyhow::Result<Vec<u8>> {
    let mut encoder = ZlibEncoder::new(Vec::new(), Compression::default());
    encoder.write_all(data)?;
    Ok(encoder.finish()?)
}

pub fn decompress(data: &[u8]) -> anyhow::Result<Vec<u8>> {
    let mut decoder = ZlibDecoder::new(data);
    let mut decoded = Vec::new();
    decoder.read_to_end(&mut decoded)?;
    Ok(decoded)
}

pub fn object_path(hash: &str) -> PathBuf {
    let (dir, file) = hash.split_at(2);
    let mut path = PathBuf::from(".dvcs/objects");
    path.push(dir);
    path.push(file);
    path
}
