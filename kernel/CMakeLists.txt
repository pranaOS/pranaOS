if (ENABLE_EXTRA_KERNEL_DEBUG_SYMBOLS)
    add_compile_options(-Og)
    add_compile_options(-ggdb3)
else()
    add_compile_options(-Os)
endif()

if ("${PRANAOS_ARCH}" STREQUAL "i686")
    set(KERNEL_ARCH i386)
elseif("${PRANAOS_ARCH}" STREQUAL "x86_64")
    set(KERNEL_ARCH x86_64)
endif()

set(KERNEL_HEAP_SOURCES
    heap/SlabAllocator.cpp
    heap/kmalloc.cpp
)

set(KERNEL_SOURCES
    acpi/DynamicParser.cpp
    acpi/Initialize.cpp
    acpi/MultiProcessorParser.cpp
    acpi/Parser.cpp
    AddressSanitizer.cpp
    arch/pc/BIOS.cpp
    arch/x86/SmapDisabler.h
    bus/pci/Access.cpp
    bus/pci/Device.cpp
    bus/pci/DeviceController.cpp
    bus/pci/IOAccess.cpp
    bus/pci/MMIOAccess.cpp
    bus/pci/Initializer.cpp
    bus/pci/WindowedMMIOAccess.cpp
    bus/usb/UHCIController.cpp
    bus/usb/USBController.cpp
    bus/usb/USBDevice.cpp
    bus/usb/USBManagement.cpp
    bus/usb/USBPipe.cpp
    bus/usb/USBTransfer.cpp
    CMOS.cpp
    CommandLine.cpp
    ConsoleDevice.cpp
    CoreDump.cpp
    devices/AsyncDeviceRequest.cpp
    devices/BlockDevice.cpp
    devices/CharacterDevice.cpp
    devices/Device.cpp
    devices/FullDevice.cpp
    devices/KCOVDevice.cpp
    devices/KCOVInstance.cpp
    devices/MemoryDevice.cpp
    devices/NullDevice.cpp
    devices/PCISerialDevice.cpp
    devices/PCSpeaker.cpp
    devices/RandomDevice.cpp
    devices/SB16.cpp
    devices/SerialDevice.cpp
    devices/VMWareBackdoor.cpp
    devices/ZeroDevice.cpp
    devices/hid/I8042Controller.cpp
    devices/hid/HIDManagement.cpp
    devices/hid/KeyboardDevice.cpp
    devices/hid/MouseDevice.cpp
    devices/hid/PS2KeyboardDevice.cpp
    devices/hid/PS2MouseDevice.cpp
    devices/hid/VMWareMouseDevice.cpp
    GlobalProcessExposed.cpp
    graphics/bochs/GraphicsAdapter.cpp
    graphics/console/GenericFramebufferConsole.cpp
    graphics/console/ContiguousFramebufferConsole.cpp
    graphics/console/TextModeConsole.cpp
    graphics/console/VGAConsole.cpp
    graphics/FramebufferDevice.cpp
    graphics/GraphicsManagement.cpp
    graphics/intel/NativeGraphicsAdapter.cpp
    graphics/virtiogpu/FrameBufferDevice.cpp
    graphics/virtiogpu/Console.cpp
    graphics/virtiogpu/GPU.cpp
    graphics/virtiogpu/GraphicsAdapter.cpp
    graphics/VGACompatibleAdapter.cpp
    SanCov.cpp
    storage/partition/DiskPartition.cpp
    storage/partition/DiskPartitionMetadata.cpp
    storage/partition/EBRPartitionTable.cpp
    storage/partition/GUIDPartitionTable.cpp
    storage/partition/MBRPartitionTable.cpp
    storage/partition/PartitionTable.cpp
    storage/StorageDevice.cpp
    storage/AHCIController.cpp
    storage/AHCIPort.cpp
    storage/AHCIPortHandler.cpp
    storage/SATADiskDevice.cpp
    storage/BMIDEChannel.cpp
    storage/IDEController.cpp
    storage/IDEChannel.cpp
    storage/PATADiskDevice.cpp
    storage/RamdiskController.cpp
    storage/RamdiskDevice.cpp
    storage/StorageManagement.cpp
    DoubleBuffer.cpp
    filesystem/AnonymousFile.cpp
    filesystem/BlockBasedFileSystem.cpp
    filesystem/Custody.cpp
    filesystem/DevFS.cpp
    filesystem/DevPtsFS.cpp
    filesystem/Ext2FileSystem.cpp
    filesystem/FIFO.cpp
    filesystem/File.cpp
    filesystem/FileBackedFileSystem.cpp
    filesystem/FileDescription.cpp
    filesystem/FileSystem.cpp
    filesystem/Inode.cpp
    filesystem/InodeFile.cpp
    filesystem/InodeWatcher.cpp
    filesystem/ISO9660FileSystem.cpp
    filesystem/Mount.cpp
    filesystem/Plan9FileSystem.cpp
    filesystem/ProcFS.cpp
    filesystem/SysFS.cpp
    filesystem/SysFSComponent.cpp
    filesystem/TmpFS.cpp
    filesystem/VirtualFileSystem.cpp
    FutexQueue.cpp
    interrupts/APIC.cpp
    interrupts/GenericInterruptHandler.cpp
    interrupts/IOAPIC.cpp
    interrupts/IRQHandler.cpp
    interrupts/InterruptManagement.cpp
    interrupts/PIC.cpp
    interrupts/SharedIRQHandler.cpp
    interrupts/SpuriousInterruptHandler.cpp
    interrupts/UnhandledInterruptHandler.cpp
    KBufferBuilder.cpp
    KLexicalPath.cpp
    KString.cpp
    KSyms.cpp
    memory/AddressSpace.cpp
    memory/AnonymousVMObject.cpp
    memory/InodeVMObject.cpp
    memory/MemoryManager.cpp
    memory/PageDirectory.cpp
    memory/PhysicalPage.cpp
    memory/PhysicalRegion.cpp
    memory/PhysicalZone.cpp
    memory/PrivateInodeVMObject.cpp
    memory/ProcessPagingScope.cpp
    memory/Region.cpp
    memory/RingBuffer.cpp
    memory/ScatterGatherList.cpp
    memory/SharedInodeVMObject.cpp
    memory/VMObject.cpp
    memory/VirtualRange.cpp
    memory/VirtualRangeAllocator.cpp
    MiniStdLib.cpp
    locking/Mutex.cpp
    net/E1000ENetworkAdapter.cpp
    net/E1000NetworkAdapter.cpp
    Net/IPv4Socket.cpp
    net/LocalSocket.cpp
    net/LoopbackAdapter.cpp
    net/NE2000NetworkAdapter.cpp
    net/NetworkAdapter.cpp
    net/NetworkTask.cpp
    net/NetworkingManagement.cpp
    net/RTL8139NetworkAdapter.cpp
    net/RTL8168NetworkAdapter.cpp
    net/Routing.cpp
    net/Socket.cpp
    net/TCPSocket.cpp
    net/UDPSocket.cpp
    Panic.cpp
    PerformanceEventBuffer.cpp
    Process.cpp
    ProcessExposed.cpp
    ProcessSpecificExposed.cpp
    ProcessGroup.cpp
    RTC.cpp
    Random.cpp
    Scheduler.cpp
    StdLib.cpp
    Syscall.cpp
    syscalls/anon_create.cpp
    syscalls/access.cpp
    syscalls/alarm.cpp
    syscalls/beep.cpp
    syscalls/chdir.cpp
    syscalls/chmod.cpp
    syscalls/chown.cpp
    syscalls/chroot.cpp
    syscalls/clock.cpp
    syscalls/debug.cpp
    syscalls/disown.cpp
    syscalls/dup2.cpp
    syscalls/emuctl.cpp
    syscalls/execve.cpp
    syscalls/exit.cpp
    syscalls/fcntl.cpp
    syscalls/fork.cpp
    syscalls/ftruncate.cpp
    syscalls/futex.cpp
    syscalls/get_dir_entries.cpp
    syscalls/get_stack_bounds.cpp
    syscalls/getrandom.cpp
    syscalls/getuid.cpp
    syscalls/hostname.cpp
    syscalls/ioctl.cpp
    syscalls/keymap.cpp
    syscalls/kill.cpp
    syscalls/link.cpp
    syscalls/lseek.cpp
    syscalls/mkdir.cpp
    syscalls/mknod.cpp
    syscalls/mmap.cpp
    syscalls/module.cpp
    syscalls/mount.cpp
    syscalls/open.cpp
    syscalls/perf_event.cpp
    syscalls/pipe.cpp
    syscalls/pledge.cpp
    syscalls/prctl.cpp
    syscalls/process.cpp
    syscalls/profiling.cpp
    syscalls/ptrace.cpp
    syscalls/purge.cpp
    syscalls/read.cpp
    syscalls/readlink.cpp
    syscalls/realpath.cpp
    syscalls/rename.cpp
    syscalls/rmdir.cpp
    syscalls/sched.cpp
    syscalls/select.cpp
    syscalls/sendfd.cpp
    syscalls/setpgid.cpp
    syscalls/setuid.cpp
    syscalls/shutdown.cpp
    syscalls/sigaction.cpp
    syscalls/socket.cpp
    syscalls/stat.cpp
    syscalls/statvfs.cpp
    syscalls/sync.cpp
    syscalls/sysconf.cpp
    syscalls/thread.cpp
    syscalls/times.cpp
    syscalls/ttyname.cpp
    syscalls/umask.cpp
    syscalls/uname.cpp
    syscalls/unlink.cpp
    syscalls/unveil.cpp
    syscalls/utime.cpp
    syscalls/waitid.cpp
    syscalls/inode_watcher.cpp
    syscalls/write.cpp
    tty/ConsoleManagement.cpp
    tty/MasterPTY.cpp
    tty/PTYMultiplexer.cpp
    tty/SlavePTY.cpp
    tty/TTY.cpp
    tty/VirtualConsole.cpp
    tasks/FinalizerTask.cpp
    tasks/SyncTask.cpp
    Thread.cpp
    ThreadBlockers.cpp
    ThreadTracer.cpp
    time/APICTimer.cpp
    time/HPET.cpp
    time/HPETComparator.cpp
    time/PIT.cpp
    time/RTC.cpp
    time/TimeManagement.cpp
    TimerQueue.cpp
    UBSanitizer.cpp
    UserOrKernelBuffer.cpp
    virtio/VirtIO.cpp
    virtio/VirtIOConsole.cpp
    virtio/VirtIOConsolePort.cpp
    virtio/VirtIOQueue.cpp
    virtio/VirtIORNG.cpp
    WaitQueue.cpp
    WorkQueue.cpp
    init.cpp
    kprintf.cpp
)

set(KERNEL_SOURCES
    ${KERNEL_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/${KERNEL_ARCH}/ASM_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/${KERNEL_ARCH}/Boot/ap_setup.S
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/${KERNEL_ARCH}/InterruptEntry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/${KERNEL_ARCH}/Processor.cpp
)

set(KERNEL_SOURCES
    ${KERNEL_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/ASM_wrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/CPU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/Interrupts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/Processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/ProcessorInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/SafeMem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/common/TrapFrame.cpp
)

set(BASE_SOURCES
    ../base/FlyString.cpp
    ../base/GenericLexer.cpp
    ../base/Hex.cpp
    ../base/String.cpp
    ../base/StringBuilder.cpp
    ../base/StringImpl.cpp
    ../base/StringUtils.cpp
    ../base/StringView.cpp
    ../base/Time.cpp
    ../base/Format.cpp
    ../base/UUID.cpp
    ../base/Utf8View.cpp
    ../base/Utf16View.cpp
)

set(ELF_SOURCES
    ../userland/libraries/libelf/Image.cpp
    ../userland/libraries/libelf/Validation.cpp
)

generate_state_machine(../userland/libraries/libvt/StateMachine.txt ../userland/libraries/libvt/EscapeSequenceStateMachine.h)

set(VT_SOURCES
    ../userland/libraries/libvt/Terminal.cpp
    ../userland/libraries/libvt/Line.cpp
    ../userland/libraries/libvt/EscapeSequenceParser.cpp
)

set(KEYBOARD_SOURCES
    ../userland/libraries/libkeyboard/CharacterMap.cpp
)

set(CRYPTO_SOURCES
    ../userland/libraries/libcrypto/Cipher/AES.cpp
    ../userland/libraries/libcrypto/Hash/SHA2.cpp
)

set(SOURCES
    ${KERNEL_SOURCES}
    ${BASE_SOURCES}
    ${ELF_SOURCES}
    ${VT_SOURCES}
    ${KEYBOARD_SOURCES}
    ${CRYPTO_SOURCES}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wvla -Wnull-dereference")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -ffreestanding -fbuiltin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-80387 -mno-mmx -mno-sse -mno-sse2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-asynchronous-unwind-tables")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nodefaultlibs -nostdlib")

if (USE_CLANG_TOOLCHAIN)
    add_compile_options(-Waddress-of-packed-member)
endif()

if (NOT USE_CLANG_TOOLCHAIN AND CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 11.1)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fzero-call-used-regs=used-gpr")
endif()

if (NOT USE_CLANG_TOOLCHAIN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc -nostdinc++")
endif()

macro (set_new_alignment alignment)
    if (USE_CLANG_TOOLCHAIN)
        add_compile_options(-faligned-allocation)
        add_compile_options(-fnew-alignment=${alignment})
    else()
        add_compile_options(-faligned-new=${alignment})
    endif()
endmacro()

if ("${PRANAOS_ARCH}" STREQUAL "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=large -mno-red-zone")
    set_new_alignment(8)
else()
    set_new_alignment(4)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-pie")

if (ENABLE_KERNEL_COVERAGE_COLLECTION)
    add_definitions(-DENABLE_KERNEL_COVERAGE_COLLECTION)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-coverage=trace-pc")
    set(KCOV_EXCLUDED_SOURCES

        ../base/Format.cpp
        ../base/StringBuilder.cpp
        ../kernel/arch/x86/${KERNEL_ARCH}/Processor.cpp
        ../kernel/devices/KCOVDevice.cpp
        ../kernel/devices/KCOVInstance.cpp
        ../kernel/filesystem/File.cpp
        ../kernel/filesystem/FileDescription.cpp
        ../kernel/heap/SlabAllocator.cpp
        ../kernel/init.cpp
        ../kernel/SanCov.cpp
        ../kernel/arch/x86/common/Interrupts.cpp
        ../kernel/Syscall.cpp
    )
    set_source_files_properties(${KCOV_EXCLUDED_SOURCES} PROPERTIES COMPILE_FLAGS "-fno-sanitize-coverage=trace-pc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")

if (ENABLE_KERNEL_ADDRESS_SANITIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=kernel-address")
endif()

add_compile_definitions(KERNEL)

add_compile_definitions(__pranaos__)

if (USE_CLANG_TOOLCHAIN)
    add_link_options(LINKER:-z,notext)
    add_link_options(LINKER:--build-id=none)
endif()

add_library(kernel_heap STATIC ${KERNEL_HEAP_SOURCES})

if (${CMAKE_HOST_SYSTEM_NAME} MATCHES pranaOS)
    include_directories(/usr/local/include/c++/${GCC_VERSION}/)
elseif (USE_CLANG_TOOLCHAIN)
    include_directories("${TOOLCHAIN_ROOT}/include/c++/v1")
else()
    if (NOT EXISTS ${TOOLCHAIN_ROOT}/${PRANAOS_ARCH}-pc-pranaos/include/c++/${GCC_VERSION}/)
        message(FATAL_ERROR "Toolchain version ${GCC_VERSION} appears to be missing! Please run: Meta/pranaos.sh build-toolchain")
    endif()
    include_directories(${TOOLCHAIN_ROOT}/${PRANAOS_ARCH}-pc-pranaos/include/c++/${GCC_VERSION}/)
    include_directories(${TOOLCHAIN_ROOT}/${PRANAOS_ARCH}-pc-pranaos/include/c++/${GCC_VERSION}/${PRANAOS_ARCH}-pc-pranaos/)
endif()

if (NOT USE_CLANG_TOOLCHAIN)
    link_directories(${TOOLCHAIN_ROOT}/${PRANAOS_ARCH}-pc-pranaos/lib)
    link_directories(${TOOLCHAIN_ROOT}/lib/gcc/${PRANAOS_ARCH}-pc-pranaos/${GCC_VERSION}/)
endif()

add_executable(kernel ${SOURCES})
add_dependencies(kernel generate_EscapeSequenceStateMachine.h)

add_custom_command(
    OUTPUT linker.ld
    COMMAND "${CMAKE_CXX_COMPILER}" -E -P -x c -I${CMAKE_CURRENT_SOURCE_DIR}/.. "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld" -o "${CMAKE_CURRENT_BINARY_DIR}/linker.ld"
    MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
    COMMENT "Preprocessing linker.ld"
    VERBATIM
)

add_custom_target(generate_kernel_linker_script DEPENDS linker.ld)

target_link_options(kernel PRIVATE LINKER:-T ${CMAKE_CURRENT_BINARY_DIR}/linker.ld -nostdlib)
set_target_properties(kernel PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/linker.ld")

if (ENABLE_KERNEL_LTO)
    include(CheckIPOSupported)
    check_ipo_supported()
    set_property(TARGET kernel PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if (USE_CLANG_TOOLCHAIN)
    target_link_libraries(kernel kernel_heap clang_rt.builtins-${PRANAOS_CLANG_ARCH})
else()
    target_link_libraries(kernel kernel_heap gcc)
endif()

add_dependencies(kernel kernel_heap)

add_custom_command(
    TARGET kernel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env CXXFILT=${CMAKE_CXXFILT} sh ${CMAKE_CURRENT_SOURCE_DIR}/mkmap.sh
    COMMAND ${CMAKE_COMMAND} -E env OBJCOPY=${CMAKE_OBJCOPY} sh ${CMAKE_CURRENT_SOURCE_DIR}/embedmap.sh
    COMMAND ${CMAKE_OBJCOPY} --only-keep-debug kernel kernel.debug
    COMMAND ${CMAKE_OBJCOPY} --strip-debug kernel
    COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=kernel.debug kernel
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/kernel.map
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kernel" DESTINATION boot)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kernel.debug" DESTINATION boot)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kernel.map" DESTINATION res)

pranaos_install_headers(kernel)
pranaos_install_sources(kernel)

add_subdirectory(prekernel)
add_subdirectory(modules)
