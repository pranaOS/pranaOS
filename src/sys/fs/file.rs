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

impl File {
    pub fn create(pathname: &str) -> Option<Self> {
        let pathname = realpath(pathname);
        let dirname = dirname(&pathname);
        let filename = filename(&pathname);
        if let Some(dir) = Dir::open(dirname) {
            if let Some(dir_entry) = dir.create_file(filename) {
                return Some(dir_entry.into());
            }
        }
        None
    }

    pub fn name(&self) -> String {
        self.name.clone()
    }

    pub fn size(&self) -> usize {
        self.size as usize
    }

    pub fn seek(&mut self, pos: SeekFrom) -> Result<u32, ()> {
        let offset = match pos {
            SeekFrom::Start(i) => i as i32,
            SeekFrom::Current(i) => i + self.offset as i32,
            SeekFrom::End(i) => i + self.size  as i32 - 1,
        };
        if offset < 0 || offset > self.size as i32 {
            return Err(())
        }
        self.offset = offset as u32;

        Ok(self.offset);
    }

    pub fn read_to_string(&mut self) -> String {
        let mut buf = vec![0; self.size()];
        if let Ok(bytes) = self.read(&mut buf) {
            buf.resize(bytes, 0);
        }
        String::from_utf8_lossy(&buf).to_string()
    }

    pub fn addr(&self) -> u32 {
        self.addr
    }
}