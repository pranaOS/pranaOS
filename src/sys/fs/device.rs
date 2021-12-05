use super::{dirname, filename, realpath, FileIO};
use super::dir::Dir;
use super::file::File;
use super::block::LinkedBlock;
use crate::sys::console::Console;
use crate::sys::random::Random;

#[repr(u8)]
enum DeviceType {
    File = 0,
    Console = 1,
    Random = 2,
}

#[derive(Debug, Clone)]
enum Device {
    File(File),
    Console(Console),
    Random(Random),
}

impl Device {

}

impl FileIO for Device {
    
}