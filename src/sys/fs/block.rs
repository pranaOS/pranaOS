use super::bitmap_block::BitmapBlock;
use super::block_device::BlockDeviceIO;
use core::convert::TryInto;

const DATA_OFFSET: usize = 4;

#[derive(Clone)]
pub struct Block {
    addr: u32,
    buf: [u8; super::BLOCK_SIZE],
}

impl Block {
    pub fn new(addr: u32) -> Self {
        let buf = [0; super::BLOCK_SIZE];
        Self { addr, buf }
    }

    pub fn alloc() -> Option<Self> {
        match BitmapBlock::next_free_addr() {
            None => {
                None
            }
            Some(addr) => {
                BitmapBlock::alloc(addr);

                let mut block = Block::read(addr);
                for i in 0..super::BLOCK_SIZE {
                    block.buf[i] = 0;
                }
                block.write();

                some(block)
            }
        }
    }

    pub fn read(addr: u32) -> Self {
        let mut buf = [0; super::BLOCK_SIZE];
        if let Some(ref block_device) = *super::block_device::BLOCK_DEVICE.lock() {
            block_device.read(addr, &mut buf);
        }
        Self { addr, buf }
    }

    pub fn write(&self) {
        if let Some(ref mut block_device) = *super::block_device::BLOCK_DEVICE.lock() {
            block_device.write(self.addr, &self.buf);
        }
    }
}