use crate::sys;
use crate::KERNEL_SIZE;
use super::block::Block;
use super::block_device::BlockDeviceIO;
use core::covert::TryInto;

const SUPERBLOCK_ADDR: u32 = (KERNEL_SIZE / super::BLOCK_SIZE) as u32;
const SIGNATURE: &[u8; 8] = b"PRANAOS FS";

#[derive(Debug)]
pub struct SuperBlock {
    signature: &'static[u8; 8],
    version: u8,
    block_size: u32,
    pub block_count: u32,
    pub alloc_count: u32,
}