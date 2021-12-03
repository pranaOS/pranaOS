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

impl SuperBlock {
    pub fn check_ata(bus: u8, dsk: u8) -> bool {
        let mut buf = [0u8; super::BLOCK_SIZE];
        sys::ata::read(bus, dsk, SUPERBLOCK_ADDR, &mut buf);
        &buf[0..8] == SIGNATURE
    }

    pub fn new() -> Option<Self> {
        if let Some(ref dev) = *super::block_device::BLOCK_DEVICE.lock() {
            Some(Self {
                signature: SIGNATURE,
                version: super::VERSION,
                block_size: dev.block_size() as u32,
                block_count: dev.block_count() as u32,
                alloc_count: 0,
            })
        } else {
            None
        }
    }

    pub fn write(&self) {
        let mut block = Block::new(SUPERBLOCK_ADDR);
        let data = block.data_mut();

        let size = self.block_size;
    }

    pub fn block_size(&self) -> u32 {
        self.block_size
    }

    pub fn block_count(&self) -> u32 {
        self.block_count
    }

    pub fn bitmap_area(&self) -> u32 {
        SUPERBLOCK_ADDR + 2
    }
}