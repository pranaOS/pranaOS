# pranaOS
Operating System for x86, ARM, mobile, and watch platforms. It provides a robust kernel, a graphical user interface, and a comprehensive set of libraries for application development. With a focus on performance, security, and modularity, pranaOS aims to be a versatile OS for various computing environments.

<p align="center">
<img src="https://raw.githubusercontent.com/pranaOS/pranaOS/master/imgs/pranaOS.png" width="200" height="200">
</p>

[![macCI](https://github.com/pranaOS/pranaOS/actions/workflows/macos.yml/badge.svg?branch=master)](https://github.com/pranaOS/pranaOS/actions/workflows/macos.yml)

[![linuxCI](https://github.com/pranaOS/pranaOS/actions/workflows/linux.yml/badge.svg?branch=master)](https://github.com/pranaOS/pranaOS/actions/workflows/linux.yml)

# Features:
- operating system for all, there are many features at this operating system, you can able to create frameworks, applications, and many more for the os(watch, mobile & desktopOS).
- we have specialized ui & other runtime libraries so that developers can able to develop many things using it (or) they can create they're own framework or application for the pranaOS.
- we have our own distributed version control system and our own programming language

## Kernel
- **Multi-threaded Kernel**: Optimized for x86, ARM, mobile, and watch architectures.
- **Memory Management**: Efficient handling of system memory with paging support.
- **Filesystem Support**: Implements Ext2 with `/proc` virtual filesystem.
- **Networking**: Supports IPv4 with ARP, TCP, UDP, and ICMP protocols.
- **Device Drivers**:
  - PS/2 Mouse & Keyboard support
  - 16-bit sound drivers
  - Mouse integration for Virtual Machines

## Libraries:
- gateway & entrypoint **(mods)**
- objective-c runtime support **(libobjc)**
- swift runtime support **(libswift)**
- java runtime and support for kernel **(libjava)**
- Application runtime(swift/c++/objective-c)

## Userland
- **POSIX Compatibility**: Supports standard Unix utilities, signals, and `/proc` filesystem.
- **Graphical User Interface**: A fully customizable UI with theming support.
- **Essential System Applications**: Includes a terminal, file manager, text editor, and calculator.
- **Security & Performance**:
  - Hardware security integration
  - Filesystem notifiers
  - System profiling tools

## Development & Libraries
- **Custom UI & Runtime Libraries**: Provides a specialized UI toolkit and runtime libraries for application development.
- **Multi-language Support**:
  - **libobjc**: Objective-C runtime support
  - **libswift**: Swift runtime support
  - **libjava**: Java runtime with kernel integration
- **Application Development**:
  - Supports Swift and C++ applications
  - Provides a gateway and entry point library (Mods)

# Requirements:
- you need prana compiler 
- to build prana check out the [doc]()

## Todo:
- check out the [todo's](https://github.com/pranaOS/pranaOS/projects/6)

## Have Any Issues??:
- contact our [discord](https://discord.gg/XmpBTmy9Bz)

## Social Media:
- [Youtube](https://www.youtube.com/channel/UCHBR5EYG9MDJ-yws4zN6xXQ)
- [Discord](https://discord.gg/XmpBTmy9Bz)

## Contribtuions
- Intersted in this project??
- pranaOS is an open-source project you can contribute to it :)
- check out the [CONTRIBUTING](CONTRIBUTING.md) docs


## Contributors:
- Thanks for those who contributed to this project :)
<br>
 <a href="https://github.com/pranaOS/pranaOS/graphs/contributors">
   <img src="https://contributors-img.web.app/image?repo=pranaOS/pranaOS" />
</a>

## Inspired by:
- [macOS](https://github.com/apple/darwin-xnu)
- [Linux](https://github.com/torvalds/linux)

## Special mentions:
- thanks for those who [stared](https://github.com/pranaOS/pranaOS/stargazers) ⭐ this project and gave a motivation :)
- special thanks for those who has [contributed](https://github.com/pranaOS/pranaOS/graphs/contributors) and still contributing to this project
- thanks to OSDev wiki it helped me a lot!!!

- [NukeWilliams](https://github.com/NukeWilliams) who continously contributed to this project and added usefull things
- [HirotoHinta](https://github.com/HirotoHinta) He helped a lot in kernel side bringed up new features to the project!
- [OliviaCE](https://github.com/OliviaCE) OlivaCE helped a lot in kernel
- [evilbat831](https://github.com/evilbat831) Henry worked in libraries, kernel
- [biggboy021](https://github.com/biggboy021) Chester is working in libraries

## License:
- pranaOS is licensed under [GNU](https://github.com/pranaOS/pranaOS/blob/master/LICENSE)
