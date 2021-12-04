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

    pub fn alloc(addr: u32) {
        let mut block = Block::read(BitmapBlock::block_index(addr));
        let bitmap = block.data_mut();
        let i = BitmapBlock::buffer_index(addr);
        if !bitmap[i / 8].get_bit(i % 8) {
            bitmap[i / 8].set_bit(i % 8, true);
            blcok.write();
            super_block::inc_alloc_count();
        }
    }

    pub fn free(addr: u32) {
        let mut block = Block::read(BitmapBlock::block_index(addr));
        let bitmap = block.data_mut();
        let i = BitmapBlock::buffer_index(addr);
        bitmap[i / 8].set_bit(i % 8, false);
        block.write();
        super_block::dec_alloc_count();
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