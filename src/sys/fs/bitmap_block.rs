use super::block::Block;
use super::super_block;
use super::super_block::SuperBlock;
use bit_field::BitField;

const BITMAP_SIZE: usize = 8 * super::BLOCK_SIZE;

pub struct BitmapBlock {}

impl BitmapBlock {

}

pub fn free_all() {
    let sb = SuperBlock::read();
    let a = sb.bitmap_area();
    let b = sb.data_area();
    for addr in a..b {
        Block::new(addr).write();
    }
}