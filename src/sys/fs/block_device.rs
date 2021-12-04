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

impl MemBlockDevice {
    pub fn new(len: usize) -> Self {
        let dev = vec![[0; super::BLOCK_SIZE]; len];
        Self { dev }
    }
}

impl BlockDeviceIO for MemBlockDevice {
    fn read(&self, block_index: u32, buf: &mut [u8]) {
        buf[..].clone_from_slice(&self.dev[block_index as usize][..]);
    }

    fn write(&mut self, block_index: u32, buf: &[u8]) {
        self.dev[block_index as usize][..].clone_from_slice(&buf[..]);
    }

    fn block_size(&self) -> usize {
        super::BLOCK_SIZE
    }

    fn block_count(&self) -> usize {
        self.dev.len()
    }
}

pub fn mount_mem() {
    let mem = sys::allocator::memory_size() / 2;
    let len = mem / super::BLOCK_SIZE;
    let dev = MemBlockDevice::new(len);
    *BLOCK_DEVICE.lock() = Some(BlockDevice::Mem(dev));
}

#[derive(Clone)]
pub struct AtaBlockDevice {
    dev: sys::ata::Drive
}

pub fn is_mounted() {

}

pub fn dismount() {
    
}

#[test_case]
fn test_mount_mem() {
    assert!(!is_mounted());
    mount_mem();
    assert!(is_mounted());
    dismount();
}