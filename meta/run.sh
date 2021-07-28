#!/bin/sh

set -e

die() {
    echo "die: $*"
    exit 1
}


[ -e /dev/kvm ] && [ -r /dev/kvm ] && [ -w /dev/kvm ] && PRANAOS_VIRT_TECH_ARG="-enable-kvm"

[ -z "$PRANAOS_BOCHS_BIN" ] && PRANAOS_BOCHS_BIN="bochs"

if [ "$(uname)" = "Darwin" ] && [ "$(uname -m)" = "x86_64" ]; then

    [ -z "$PRANAOS_QEMU_BIN" ] && PRANAOS_QEMU_BIN="qemu-system-x86_64"

    if $PRANAOS_QEMU_BIN --accel help | grep -q hvf; then
        PRANAOS_VIRT_TECH_ARG="--accel hvf"
    fi
fi

SCRIPT_DIR="$(dirname "${0}")"

PATH="$SCRIPT_DIR/../toolchain/Local/qemu/bin:$PATH"

PATH="$SCRIPT_DIR/../toolchain/Local/i686/bin:$PATH"

PRANAOS_RUN="${PRANAOS_RUN:-$1}"

if [ -z "$PRANAOS_QEMU_BIN" ]; then
    if [ "$PRANAOS_ARCH" = "x86_64" ]; then
        PRANAOS_QEMU_BIN="qemu-system-x86_64"
    else
        PRANAOS_QEMU_BIN="qemu-system-i386"
    fi
fi

[ -z "$PRANAOS_KERNEL_CMDLINE" ] && PRANAOS_KERNEL_CMDLINE="hello"

[ -z "$PRANAOS_RAM_SIZE" ] && PRANAOS_RAM_SIZE=512M

if command -v wslpath >/dev/null; then
    case "$PRANAOS_QEMU_BIN" in
        /mnt/?/*)
            [ -z "$PRANAOS_QEMU_CPU" ] && PRANAOS_QEMU_CPU="max,vmx=off"
            PRANAOS_KERNEL_CMDLINE="$PRANAOS_KERNEL_CMDLINE disable_virtio"
    esac
fi

[ -z "$PRANAOS_QEMU_CPU" ] && PRANAOS_QEMU_CPU="max"

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
    die "Please install QEMU version 5.0 or newer or use the toolchain/BuildNewQemu.sh script."
fi

PRANAOS_QEMU_MIN_REQ_VERSION=5
installed_major_version=$("$PRANAOS_QEMU_BIN" -version | head -n 1 | sed -E 's/QEMU emulator version ([1-9][0-9]*|0).*/\1/')
if [ "$installed_major_version" -lt "$PRANAOS_QEMU_MIN_REQ_VERSION" ]; then
    echo "Required QEMU >= 5.0! Found $($PRANAOS_QEMU_BIN -version | head -n 1)"
    echo "Please install a newer version of QEMU or use the toolchain/BuildNewQemu.sh script."
    die
fi

if [ -z "$PRANAOS_SPICE" ] && "${PRANAOS_QEMU_BIN}" -chardev help | grep -iq qemu-vdagent; then
    PRANAOS_SPICE_SERVER_CHARDEV="-chardev qemu-vdagent,clipboard=on,mouse=off,id=vdagent,name=vdagent"
elif "${PRANAOS_QEMU_BIN}" -chardev help | grep -iq spicevmc; then
    PRANAOS_SPICE_SERVER_CHARDEV="-chardev spicevmc,id=vdagent,name=vdagent"
fi

if [ "$(uname)" = "Darwin" ]; then
    PRANAOS_AUDIO_BACKEND="-audiodev coreaudio,id=snd0"
else
    PRANAOS_AUDIO_BACKEND="-audiodev sdl,id=snd0"
fi

PRANAOS_SCREENS="${PRANAOS_SCREENS:-1}"
if  [ "$PRANAOS_SPICE" ]; then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-spice-app}"
elif (uname -a | grep -iq WSL) || (uname -a | grep -iq microsoft); then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-sdl,gl=off}"
elif [ $PRANAOS_SCREENS -gt 1 ] && "${PRANAOS_QEMU_BIN}" --display help | grep -iq sdl; then
    PRANAOS_QEMU_DISPLAY_BACKEND="${PRANAOS_QEMU_DISPLAY_BACKEND:-sdl,gl=off}"
elif ("${PRANAOS_QEMU_BIN}" --display help | grep -iq sdl) && (ldconfig -p | grep -iq virglrenderer); then
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
-smp 2
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
-machine pcspk-audiodev=snd0
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
-smp 2
-device pcie-root-port,port=0x10,chassis=1,id=pcie.1,bus=pcie.0,multifunction=on,addr=0x2
-device pcie-root-port,port=0x11,chassis=2,id=pcie.2,bus=pcie.0,addr=0x2.0x1
-device pcie-root-port,port=0x12,chassis=3,id=pcie.3,bus=pcie.0,addr=0x2.0x2
-device pcie-root-port,port=0x13,chassis=4,id=pcie.4,bus=pcie.0,addr=0x2.0x3
-device pcie-root-port,port=0x14,chassis=5,id=pcie.5,bus=pcie.0,addr=0x2.0x4
-device pcie-root-port,port=0x15,chassis=6,id=pcie.6,bus=pcie.0,addr=0x2.0x5
-display $PRANAOS_QEMU_DISPLAY_BACKEND
-device $PRANAOS_QEMU_DISPLAY_DEVICE
-device secondary-vga
-device bochs-display,bus=pcie.6,addr=0x10.0x0
-device piix3-ide
-drive file=${PRANAOS_DISK_IMAGE},id=disk,if=none
-device ahci,id=ahci
-device ide-hd,bus=ahci.0,drive=disk,unit=0
-usb
-device virtio-serial
-chardev stdio,id=stdout,mux=on
-device virtconsole,chardev=stdout
-device isa-debugcon,chardev=stdout
-device virtio-rng-pci
-soundhw pcspk
-device sb16
"

export SDL_VIDEO_X11_DGAMOUSE=0

: "${PRANAOS_BUILD:=.}"
cd -P -- "$PRANAOS_BUILD" || die "Could not cd to \"$PRANAOS_BUILD\""

if [ "$PRANAOS_RUN" = "b" ]; then
    [ -z "$PRANAOS_BOCHSRC" ] && {
        [ -z "$PRANAOS_SOURCE_DIR" ] && die 'PRANAOS_SOURCE_DIR not set or empty'
        PRANAOS_BOCHSRC="$PRANAOS_SOURCE_DIR/meta/bochsrc"
    }
    "$PRANAOS_BOCHS_BIN" -q -f "$PRANAOS_BOCHSRC"
elif [ "$PRANAOS_RUN" = "qn" ]; then
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE \
        -kernel kernel/prekernel/prekernel \
        -initrd kernel/kernel \
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
        -kernel kernel/prekernel/prekernel \
        -initrd kernel/kernel \
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

    echo "Starting PRANAOSOS with QEMU Q35 machine, Commandline: ${PRANAOS_KERNEL_CMDLINE}"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_Q35_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23 \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE,netdev=breh \
        -kernel kernel/prekernel/prekernel \
        -initrd kernel/kernel \
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
        -smp 2 \
        -drive file=${PRANAOS_DISK_IMAGE},format=raw,index=0,media=disk \
        -device ich9-ahci \
        -nographic \
        -display none \
        -debugcon file:debug.log \
        -kernel kernel/prekernel/prekernel \
        -initrd kernel/kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
else
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23,hostfwd=tcp:127.0.0.1:8000-10.0.2.15:8000,hostfwd=tcp:127.0.0.1:2222-10.0.2.15:22 \
        -device $PRANAOS_ETHERNET_DEVICE_TYPE,netdev=breh \
        -kernel kernel/prekernel/prekernel \
        -initrd kernel/kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
fi