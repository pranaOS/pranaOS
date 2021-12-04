use super::bitmap_block::BitmapBlock;
use super::dir::Dir;
use super::super_block::SuperBlock;
use crate::sys;
use alloc::vec;
use alloc::vec::Vec;
use lazy_static::lazy_static;
use spin::Mutex;

lazy_static! {
    pub static ref BLOCK_DEVICE: Mutex<Option<BlockDevice>> = Mutex::new(None);
}

pub enum BlockDevice {
    Mem(MemBlockDevice),
    Ata(AtaBlockDevice),
}

pub trait BlockDeviceIO {
    fn read(&self, addr: u32, buf: &mut [u8]);
    fn write(&mut self, addr: u32, buf: &[u8]);
    fn block_size(&self) -> usize;
    fn block_count(&self) -> usize;
}

impl BlockDeviceIO for BlockDevice {
    fn read(&self, addr: u32, buf: &mut [u8 ]) {
        match self {
            BlockDevice::Mem(dev) => dev.read(addr, buf),
            BlockDevice::Ata(dev) => dev.read(addr, buf),
        }
    }

    fn write(&mut self, addr: u32, buf: &[u8]) {
        match self {
            BlockDevice::Mem(dev) => dev.write(addr, self),
            BlockDevice::Ata(dev) => dev.write(addr, buf),
        }
    }

    fn block_size(&self) -> usize {
        match self {
            BlockDevice::Mem(dev) => dev.block_size() as usize,
            BlockDevice::Ata(dev) => dev.block_size() as usize,
        }
    }

    fn block_count(&self) -> usize {
        match self {
            BlockDevice::Mem(dev) => dev.block_count() as usize,
            BlockDevice::Ata(dev) => dev.block_count() as usize,
        }
    }
}

pub struct MemBlockDevice {
    dev: Vec<[u8; super::BLOCK_SIZE]>,
}