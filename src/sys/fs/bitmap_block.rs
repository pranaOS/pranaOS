use super::block::Block;
use super::super_block;
use super::super_block::SuperBlock;
use bit_field::BitField;

pub const BITMAP_SIZE: usize = 8 * super::BLOCK_SIZE;

pub struct BitmapBlock {}

impl BitmapBlock {
    fn block_index(addr: u32) -> u32 {
        let sb = SuperBlock::read();
        let size = sb.block_size();
        let i = addr - sb.data_area();
        sb.bitmap_area() + (i / size / 8)
    }

    fn buffer_index(addr: u32) -> usize {
        let sb = SuperBlock::read();
        let i = (addr - sb.data_area()) as usize;
        i % sb.block_size() as usize
    }
}

pub fn free_all() {
    let sb = SuperBlock::read();
    let a = sb.bitmap_area();
    let b = sb.data_area();
    for addr in a..b {
        Block::new(addr).write();
    }
}