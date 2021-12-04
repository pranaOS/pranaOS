use super::{dirname, filename, realpath, FileIO};
use super::dir::Dir;
use super::block::LinkedBlock;
use super::dir_entry::DirEntry;
use alloc::string::{String, ToString};
use alloc::vec;
use core::convert::From;

pub enum SeekFrom {
    Start(u32),
    Current(i32),
    End(i32),
}

#[derive(Debug, Clone)]
pub struct File {
    name: String,
    addr: u32,
    size: u32,
    dir: Dir,
    offset: u32,
}

impl From<DirEntry> for File {
    fn from(entry: DirEntry) -> Self {
        Self {
            name: entry.name(),
            addr: entry.addr(),
            size: entry.size(),
            dir: entry.dir(),
            offset: 0,
        }
    }
}