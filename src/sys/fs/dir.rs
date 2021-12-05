use super::{dirname, filename, realpath, FileIO};
use super::super_block::SuperBlock;
use super::dir_entry::DirEntry;
use super::read_dir::ReadDir;
use super::bitmap_block::BitmapBlock;
use super::FileType;
use super::block::LinkedBlock;
use crate::sys;
use alloc::string::String;
use core::convert::From;

#[derive(Debug, Clone, Copy)]
pub struct Dir {
    addr: u32,
}

impl From<DirEntry> for Dir {
    fn from(entry: DirEntry) -> Self {
        Self { addr: entry.addr() }
    }
}

impl Dir {
    pub fn root() -> Self {
        Self { addr: SuperBlock::read().data_area() }
    }
}

impl FileIO for Dir {
    fn read(&mut self, _buf: &mut [u8]) -> Result<usize, ()> {
        Err(())
    }
    fn write(&mut self, _buf: &[u8]) -> Result<usize, ()> {
        Err(())
    }
}

fn truncate(s: &str, max: usize) -> String {
    s.char_indices().take_while(|(i, _)| *i <= max).map(|(_, c)| c).collect()
}