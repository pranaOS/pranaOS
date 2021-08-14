#!/bin/sh

set -e

die() {
    echo "die: $*"
    exit 1
}

SCRIPT_DIR="$(dirname "${0}")"

[ -x "$SCRIPT_DIR/../run-local.sh" ] && . "$SCRIPT_DIR/../run-local.sh"

KVM_SUPPORT="0"
[ -e /dev/kvm ] && [ -r /dev/kvm ] && [ -w /dev/kvm ] && KVM_SUPPORT="1"

[ -z "$PRANAOS_BOCHS_BIN" ] && PRANAOS_BOCHS_BIN="bochs"

if [ "$(uname)" = "Darwin" ] && [ "$(uname -m)" = "x86_64" ]; then

    [ -z "$PRANAOS_QEMU_BIN" ] && PRANAOS_QEMU_BIN="qemu-system-x86_64"

    if $PRANAOS_QEMU_BIN --accel help | grep -q hvf; then
        PRANAOS_VIRT_TECH_ARG="--accel hvf"
    fi
fi

PATH="$SCRIPT_DIR/../toolchain/Local/qemu/bin:$PATH"

PATH="$SCRIPT_DIR/../toolchain/Local/i686/bin:$PATH"

PRANAOS_RUN="${PRANAOS_RUN:-$1}"

if [ -z "$PRANAOS_QEMU_BIN" ]; then
    if command -v wslpath >/dev/null; then
        QEMU_INSTALL_DIR=$(reg.exe query 'HKLM\Software\QEMU' /v Install_Dir /t REG_SZ | grep '^    Install_Dir' | sed 's/    / /g' | cut -f4- -d' ')
        if [ -z "$QEMU_INSTALL_DIR" ]; then
            if [ "$KVM_SUPPORT" -eq "0" ]; then
                die "Could not determine where QEMU for Windows is installed. Please make sure QEMU is installed or set PRANAOS_QEMU_BIN if it is already installed."
            fi
        else
            KVM_SUPPORT="0"
            QEMU_BINARY_PREFIX="$(wslpath -- "${QEMU_INSTALL_DIR}" | tr -d '\r\n')/"
            QEMU_BINARY_SUFFIX=".exe"
        fi
    fi
    if [ "$PRANAOS_ARCH" = "x86_64" ]; then
        PRANAOS_QEMU_BIN="${QEMU_BINARY_PREFIX}qemu-system-x86_64${QEMU_BINARY_SUFFIX}"
    else
        PRANAOS_QEMU_BIN="${QEMU_BINARY_PREFIX}qemu-system-i386${QEMU_BINARY_SUFFIX}"
    fi
fi

[ "$KVM_SUPPORT" -eq "1" ] && PRANAOS_VIRT_TECH_ARG="-enable-kvm"

[ -z "$PRANAOS_KERNEL_CMDLINE" ] && PRANAOS_KERNEL_CMDLINE="hello"

[ -z "$PRANAOS_RAM_SIZE" ] && PRANAOS_RAM_SIZE=512M

[ -z "$PRANAOS_DISK_IMAGE" ] && {
    if [ "$PRANAOS_RUN" = qgrub ]; then
        PRANAOS_DISK_IMAGE="grub_disk_image"
    elif [ "$PRANAOS_RUN" = qextlinux ]; then
        PRANAOS_DISK_IMAGE="extlinux_disk_image"
    else
        PRANAOS_DISK_IMAGE="_disk_image"
    fi
    if command -v wslpath >/dev/null; then
        case "$PRANAOS_QEMU_BIN" in
            /mnt/?/*)
                PRANAOS_DISK_IMAGE=$(wslpath -w "$PRANAOS_DISK_IMAGE")
                ;;
        esac
    fi
}

if ! command -v "$PRANAOS_QEMU_BIN" >/dev/null 2>&1 ; then
    die "Please install QEMU version 5.0 or newer or use the toolchain/BuildQemu.sh script."
fi

PRANAOS_QEMU_MIN_REQ_VERSION=5
installed_major_version=$("$PRANAOS_QEMU_BIN" -version | head -n 1 | sed -E 's/QEMU emulator version ([1-9][0-9]*|0).*/\1/')
installed_minor_version=$("$PRANAOS_QEMU_BIN" -version | head -n 1 | sed -E 's/QEMU emulator version [0-9]+\.([1-9][0-9]*|0).*/\1/')
if [ "$installed_major_version" -lt "$PRANAOS_QEMU_MIN_REQ_VERSION" ]; then
    echo "Required QEMU >= 5.0! Found $($PRANAOS_QEMU_BIN -version | head -n 1)"
    echo "Please install a newer version of QEMU or use the toolchain/BuildQemu.sh script."
    die
fi

NATIVE_WINDOWS_QEMU="0"

if command -v wslpath >/dev/null; then
    case "$PRANAOS_QEMU_BIN" in
        /mnt/?/*)
            if [ -z "$PRANAOS_VIRT_TECH_ARG" ]; then
                if [ "$installed_major_version" -gt 5 ]; then
                    PRANAOS_VIRT_TECH_ARG="-accel whpx,kernel-irqchip=off -accel tcg"
                else
                    PRANAOS_VIRT_TECH_ARG="-accel whpx -accel tcg"
                fi
            fi
            [ -z "$PRANAOS_QEMU_CPU" ] && PRANAOS_QEMU_CPU="max,vmx=off"
            PRANAOS_KERNEL_CMDLINE="$PRANAOS_KERNEL_CMDLINE disable_virtio"
            NATIVE_WINDOWS_QEMU="1"
            ;;
    esac
fi

[ -z "$PRANAOS_QEMU_CPU" ] && PRANAOS_QEMU_CPU="max"
[ -z "$PRANAOS_CPUS" ] && PRANAOS_CPUS="2"

if [ -z "$PRANAOS_SPICE" ] && "${PRANAOS_QEMU_BIN}" -chardev help | grep -iq qemu-vdagent; then
    PRANAOS_SPICE_SERVER_CHARDEV="-chardev qemu-vdagent,clipboard=on,mouse=off,id=vdagent,name=vdagent"
elif "${PRANAOS_QEMU_BIN}" -chardev help | grep -iq spicevmc; then
    PRANAOS_SPICE_SERVER_CHARDEV="-chardev spicevmc,id=vdagent,name=vdagent"
fi

if [ "$(uname)" = "Darwin" ]; then
    PRANAOS_AUDIO_BACKEND="-audiodev coreaudio,id=snd0"
elif [ "$NATIVE_WINDOWS_QEMU" -eq "1" ]; then
    PRANAOS_AUDIO_BACKEND="-audiodev dsound,id=snd0"
elif "$PRANAOS_QEMU_BIN" -audio-help 2>&1 | grep -- "-audiodev id=sdl" >/dev/null; then
    PRANAOS_AUDIO_BACKEND="-audiodev sdl,id=snd0"
else
    PRANAOS_AUDIO_BACKEND="-audiodev pa,id=snd0"
fi

if [ "$installed_major_version" -eq 5 ] && [ "$installed_minor_version" -eq 0 ]; then
    PRANAOS_AUDIO_HW="-soundhw pcspk"
else
    PRANAOS_AUDIO_HW="-machine pcspk-audiodev=snd0"
fi

PRANAOS_SCREENS="${PRANAOS_SCREENS:-1}"
if [ "$PRANAOS_SPICE" ]; then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-spice-app}"
elif [ "$NATIVE_WINDOWS_QEMU" -eq "1" ]; then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-sdl,gl=off}"
elif [ $PRANAOS_SCREENS -gt 1 ] && "${PRANAOS_QEMU_BIN}" --display help | grep -iq sdl; then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-sdl,gl=off}"
elif ! command -v wslpath >/dev/null && ("${PRANAOS_QEMU_BIN}" --display help | grep -iq sdl) && (ldconfig -p | grep -iq virglrenderer); then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-sdl,gl=on}"
elif "${PRANAOS_QEMU_BIN}" --display help | grep -iq cocoa; then

    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-cocoa,gl=off}"
else
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-gtk,gl=off}"
fi

if [ "$PRANAOS_SCREENS" -gt 1 ]; then
    PRANAOS_QEMU_DISPLAY_DEVICE="virtio-vga,max_outputs=$PRANAOS_SCREENS "
    
    PRANAOS_KERNEL_CMDLINE="$PRANAOS_KERNEL_CMDLINE vmmouse=off"
else
    PRANAOS_QEMU_DISPLAY_DEVICE="VGA,vgamem_mb=64 "
fi

if [ -z "$PRANAOS_DISABLE_GDB_SOCKET" ]; then
  PRANAOS_EXTRA_QEMU_ARGS="$PRANAOS_EXTRA_QEMU_ARGS -s"
fi

if [ -z "$PRANAOS_ETHERNET_DEVICE_TYPE" ]; then
  PRANAOS_ETHERNET_DEVICE_TYPE="e1000"
fi

[ -z "$PRANAOS_COMMON_QEMU_ARGS" ] && PRANAOS_COMMON_QEMU_ARGS="
$PRANAOS_EXTRA_QEMU_ARGS
-m $PRANAOS_RAM_SIZE
-cpu $PRANAOS_QEMU_CPU
-d guest_errors
-smp $PRANAOS_CPUS
-display $PRANAOS_QEMU_DISPLAY_BACKEND
-device $PRANAOS_QEMU_DISPLAY_DEVICE
-drive file=${PRANAOS_DISK_IMAGE},format=raw,index=0,media=disk
-usb
$PRANAOS_SPICE_SERVER_CHARDEV
-device virtio-serial,max_ports=2
-chardev stdio,id=stdout,mux=on
-device virtconsole,chardev=stdout
-device isa-debugcon,chardev=stdout
-device virtio-rng-pci
$PRANAOS_AUDIO_BACKEND
$PRANAOS_AUDIO_HW
-device sb16,audiodev=snd0
-device pci-bridge,chassis_nr=1,id=bridge1 -device $PRANAOS_ETHERNET_DEVICE_TYPE,bus=bridge1
-device i82801b11-bridge,bus=bridge1,id=bridge2 -device sdhci-pci,bus=bridge2
-device i82801b11-bridge,id=bridge3 -device sdhci-pci,bus=bridge3
-device ich9-ahci,bus=bridge3
"

if "${PRANAOS_QEMU_BIN}" -chardev help | grep -iq spice; then
    PRANAOS_COMMON_QEMU_ARGS="$PRANAOS_COMMON_QEMU_ARGS
    -spice port=5930,agent-mouse=off,disable-ticketing=on
    -device virtserialport,chardev=vdagent,nr=1
    "
fi

[ -z "$PRANAOS_COMMON_QEMU_Q35_ARGS" ] && PRANAOS_COMMON_QEMU_Q35_ARGS="
$PRANAOS_EXTRA_QEMU_ARGS
-m $PRANAOS_RAM_SIZE
-cpu $PRANAOS_QEMU_CPU
-machine q35
-d guest_errors
-smp $PRANAOS_CPUS
-vga none
-device bochs-display
-device ich9-usb-ehci1,bus=pcie.0,multifunction=on,addr=0x5.0x0
-device ich9-usb-ehci2,bus=pcie.0,addr=0x5.0x2
-device ich9-usb-uhci1,bus=pcie.0,multifunction=on,addr=0x7.0x0
-device ich9-usb-uhci2,bus=pcie.0,addr=0x7.0x1
-device ich9-usb-uhci3,bus=pcie.0,addr=0x7.0x2
-device ich9-usb-uhci4,bus=pcie.0,addr=0x7.0x3
-device ich9-usb-uhci5,bus=pcie.0,addr=0x7.0x4
-device ich9-usb-uhci6,bus=pcie.0,addr=0x7.0x5
-device pcie-root-port,port=0x10,chassis=1,id=pcie.1,bus=pcie.0,multifunction=on,addr=0x6
-device pcie-root-port,port=0x11,chassis=2,id=pcie.2,bus=pcie.0,addr=0x6.0x1
-device pcie-root-port,port=0x12,chassis=3,id=pcie.3,bus=pcie.0,addr=0x6.0x2
-device pcie-root-port,port=0x13,chassis=4,id=pcie.4,bus=pcie.0,addr=0x6.0x3
-device pcie-root-port,port=0x14,chassis=5,id=pcie.5,bus=pcie.0,addr=0x6.0x4
-device pcie-root-port,port=0x15,chassis=6,id=pcie.6,bus=pcie.0,addr=0x6.0x5
-device pcie-root-port,port=0x16,chassis=7,id=pcie.7,bus=pcie.0,addr=0x6.0x6
-device pcie-root-port,port=0x17,chassis=8,id=pcie.8,bus=pcie.0,addr=0x6.0x7
-device bochs-display,bus=pcie.6,addr=0x10.0x0
-device ich9-intel-hda,bus=pcie.2,addr=0x03.0x0
-device nec-usb-xhci,bus=pcie.2,addr=0x11.0x0
-device pci-bridge,chassis_nr=1,id=bridge1,bus=pcie.4,addr=0x3.0x0
-device sdhci-pci,bus=bridge1,addr=0x1.0x0
-display $PRANAOS_QEMU_DISPLAY_BACKEND
-drive file=${PRANAOS_DISK_IMAGE},format=raw,id=disk,if=none
-device ahci,id=ahci
-device ide-hd,bus=ahci.0,drive=disk,unit=0
-device virtio-serial
-chardev stdio,id=stdout,mux=on
-device virtconsole,chardev=stdout
-device isa-debugcon,chardev=stdout
-device virtio-rng-pci
$PRANAOS_AUDIO_BACKEND
$PRANAOS_AUDIO_HW
"

export SDL_VIDEO_X11_DGAMOUSE=0

: "${PRANAOS_BUILD:=.}"
cd -P -- "$PRANAOS_BUILD" || die "Could not cd to \"$PRANAOS_BUILD\""

if [ "$PRANAOS_RUN" = "b" ]; then

    [ -z "$PRANAOS_BOCHSRC" ] && {

        [ -z "$PRANAOS_SOURCE_DIR" ] && die 'PRANAOS_SOURCE_DIR not set or empty'
        PRANAOS_BOCHSRC="$PRANAOS_SOURCE_DIR/Meta/bochsrc"
    }
    "$PRANAOS_BOCHS_BIN" -q -f "$PRANAOS_BOCHSRC"
elif [ "$PRANAOS_RUN" = "qn" ]; then

    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE \
        -kernel Kernel/Prekernel/Prekernel \
        -initrd Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
elif [ "$PRANAOS_RUN" = "qtap" ]; then

    sudo ip tuntap del dev tap0 mode tap || true
    sudo ip tuntap add dev tap0 mode tap user "$(id -u)"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev tap,ifname=tap0,id=br0 \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE,netdev=br0 \
        -kernel Kernel/Prekernel/Prekernel \
        -initrd Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
    sudo ip tuntap del dev tap0 mode tap
elif [ "$PRANAOS_RUN" = "qgrub" ] || [ "$PRANAOS_RUN" = "qextlinux" ]; then

    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23 \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE,netdev=breh
elif [ "$PRANAOS_RUN" = "q35" ]; then
    
    echo "Starting pranaOS with QEMU Q35 machine, Commandline: ${PRANAOS_KERNEL_CMDLINE}"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_Q35_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23 \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE,netdev=breh \
        -kernel Kernel/Prekernel/Prekernel \
        -initrd Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
elif [ "$PRANAOS_RUN" = "ci" ]; then

    echo "Running QEMU in CI"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_EXTRA_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        -m $PRANAOS_RAM_SIZE \
        -cpu $PRANAOS_QEMU_CPU \
        -d guest_errors \
        -no-reboot \
        -smp ${PRANAOS_CPUS} \
        -drive file=${PRANAOS_DISK_IMAGE},format=raw,index=0,media=disk \
        -device ich9-ahci \
        -nographic \
        -display none \
        -debugcon file:debug.log \
        -kernel Kernel/Prekernel/Prekernel \
        -initrd Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
else

    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23,hostfwd=tcp:127.0.0.1:8000-10.0.2.15:8000,hostfwd=tcp:127.0.0.1:2222-10.0.2.15:22 \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE,netdev=breh \
        -kernel Kernel/Prekernel/Prekernel \
        -initrd Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
fi
