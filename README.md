# pranaOS
- A unix operating system built using C++

<p align="center">
<img src="https://raw.githubusercontent.com/pranaOS/pranaOS/master/imgs/eagle.jpeg" width="200" height="200">
</p>

[![macCI](https://github.com/pranaOS/pranaOS/actions/workflows/macos.yml/badge.svg?branch=master)](https://github.com/pranaOS/pranaOS/actions/workflows/macos.yml)

[![linuxCI](https://github.com/pranaOS/pranaOS/actions/workflows/linux.yml/badge.svg?branch=master)](https://github.com/pranaOS/pranaOS/actions/workflows/linux.yml)

[![codeql-analysis](https://github.com/pranaOS/pranaOS/actions/workflows/codeql-analysis.yml/badge.svg?branch=master)](https://github.com/pranaOS/pranaOS/actions/workflows/codeql-analysis.yml)

# Features:
## Kernel:
- x86 Kernel
- Filesystem
- PS2 Keyboard + USB Keyboard

## Libraries:
- runtime support for: libc, libcpp, libobjc, libjs
- ui function: libui

## Userland:
- simple terminal and some apps

# Requirements:
```i686-elf-gcc``` i686 compiler to compile this project
```make``` 
```nasm``` nasm to build the asm file

# Building:
- ```make pranaOS``` to build iso
- ```make clean``` remove the iso and remove the builded files 
- ```make qemu``` build iso and run it

## Todo:
- pranaOS installer to install pranaOS in a local hard drive
- usb support

## Have Any Issues??:
- contact our [discord](https://discord.gg/XmpBTmy9Bz)

## Social Media:
- [twitter](https://twitter.com/os_prana)
- [discord](https://discord.gg/XmpBTmy9Bz)

## Contribtuions
- pranaOS is an open-source project you can contribute to it :)
- check out the [CONTRIBUTING](CONTRIBUTING.md) docs


## Contributors:
- Thanks for those who contributed to this project :)
<br>
 <a href="https://github.com/pranaOS/pranaOS/graphs/contributors">
   <img src="https://contributors-img.web.app/image?repo=pranaOS/pranaOS" />
</a>

## Inspired by:
- [SerenityOS](https://github.com/SerenityOS/Serenity)
- [macOS](https://github.com/apple/darwin-xnu)

## pranaOS project structure
- pranaOS [project structure](https://github.com/pranaOS/pranaOS/blob/master/imgs/pranaOSProjectStrucutre.pdf) in a clean pdf

## Special mentions:
- thanks for those who [stared](https://github.com/pranaOS/pranaOS/stargazers) ⭐ this project
- special thanks for those who is [contributing](https://github.com/pranaOS/pranaOS/graphs/contributors) to this project

- [NukeWilliams](https://github.com/NukeWilliams) who continously contributed to this project and added usefull things
- [OliviaCE](https://github.com/OliviaCE) OlivaCE helped a lot in kernel
- [evilbat831](https://github.com/evilbat831) henry is a all rounder!
- [biggboy021](https://github.com/biggboy021) Chester is working in libraries

## Other projects:
- some code i took from other project
- [libobjc](https://github.com/pranaOS/pranaOS/tree/master/libs/libobjc) took from official gcc-mirror [LICENSE](https://github.com/pranaOS/pranaOS/blob/master/libs/libobjc/COPYING)

## NOTE I HAVE ADDED THE LICENSE OF THE SOURCE CODE THAT I TOOK FROM OTHER PROJECTS

## License:

<a href="https://opensource.org/licenses/gpl-license">
  <img align="right" height="96" alt="GPL License" src="imgs/gpl.png" />
</a>

- pranaOS is licensed under [GPL-3.0](https://github.com/pranaOS/pranaOS/blob/master/LICENSE)

- you can use the code in your project if you need! :)
