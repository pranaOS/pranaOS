## Project structure

- pranaOS project structure

## Ak:
- ak means all kind of files
- This is the entry point of the project and contains the headers require to build library + kernel 

## Bootloader:
- responsible for booting a pranaOS 

## Kernel:
- The kernel is the essential center of a computer operating system (OS)
- [include](https://github.com/pranaOS/pranaOS/tree/master/kernel/include) require for building kernel
- [src](https://github.com/pranaOS/pranaOS/tree/master/kernel/src) main kernel


## Libs:
- This folder contains the many libraries such as 
```
libc
libobjc
libjs
```

- libc contains basic c library
- libobjc objectivec runtime support library
- libjs javascript runtime support library

## Tests:
- unit tests