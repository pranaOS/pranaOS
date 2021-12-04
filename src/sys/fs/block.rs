use super::bitmap_block::BitmapBlock;
use super::block_device::BlockDeviceIO;
use core::convert::TryInto;

const DATA_OFFSET: usize = 4;

#[derive(Clone)]
pub struct Block {
    addr: u32,
    buf: [u8; super::BLOCK_SIZE],
}