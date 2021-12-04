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

    pub fn delete(pathname: &str) -> Result<(), ()> {
        let pathname = realpath(pathname);
        let dirname = dirname(&pathname);
        let filename = filename(&pathname);
        if let Some(mut dir) = Dir::open(dirname) {
            dir.delete_entry(filename)
        } else {
            Err(())
        }
    }
}

impl FileIO for File {
    fn read(&mut self, buf: &mut [u8]) -> Result<usize, ()> {
        let buf_len = buf.len();
        let mut addr = size.addr;
        let mut bytes = 0;
        let mut pos = 0;
        loop {
            let block = LinkedBlock::read(addr);
            let data = block.data();
            let data_len = data.len();
            for i in 0..data_len {
                if pos == self.offset {
                    if bytes == buf_len || pos as usize == self.size() {
                        return Ok(bytes);
                    }
                    buf[bytes] = data[i];
                    bytes += 1;
                    self.offset += 1;
                }
                pos += 1;
            }
            match block.next() {
                Some(next_block) => addr = next_block.addr(),
                None => return Ok(bytes),
            }
        }
    }
}

#[test_case]
fn test_file_create() {
    super::mount_mem();
    super::format_mem();
    assert!(File::create("/test").is_some());
    assert_eq!(File::create("/hello").unwrap().name(), "hello");
    super::dismount();
}