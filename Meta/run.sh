#!/bin/sh
# shellcheck disable=SC2086 # FIXME: fix these globing warnings

set -e

die() {
    echo "die: $*"
    exit 1
}

#PRANAOS_PACKET_LOGGING_ARG="-object filter-dump,id=hue,netdev=breh,file=e1000.pcap"

[ -e /dev/kvm ] && [ -r /dev/kvm ] && [ -w /dev/kvm ] && PRANAOS_VIRT_TECH_ARG="-enable-kvm"

[ -z "$PRANAOS_BOCHS_BIN" ] && PRANAOS_BOCHS_BIN="bochs"

# To support virtualization acceleration on mac
# we need to use 64-bit qemu
if [ "$(uname)" = "Darwin" ] && [ "$(uname -m)" = "x86_64" ]; then

    [ -z "$PRANAOS_QEMU_BIN" ] && PRANAOS_QEMU_BIN="qemu-system-x86_64"

    if $PRANAOS_QEMU_BIN --accel help | grep -q hvf; then
        PRANAOS_VIRT_TECH_ARG="--accel hvf"
    fi
fi

PRANAOS_RUN="${PRANAOS_RUN:-$1}"

[ -z "$PRANAOS_QEMU_BIN" ] && PRANAOS_QEMU_BIN="qemu-system-i386"

[ -z "$PRANAOS_KERNEL_CMDLINE" ] && PRANAOS_KERNEL_CMDLINE="hello"

[ -z "$PRANAOS_RAM_SIZE" ] && PRANAOS_RAM_SIZE=512M

[ -z "$PRANAOS_QEMU_CPU" ] && PRANAOS_QEMU_CPU="max"

[ -z "$PRANAOS_DISK_IMAGE" ] && {
    if [ "$PRANAOS_RUN" = qgrub ]; then
        PRANAOS_DISK_IMAGE="grub_disk_image"
    else
        PRANAOS_DISK_IMAGE="_disk_image"
    fi
}

[ -z "$PRANAOS_COMMON_QEMU_ARGS" ] && PRANAOS_COMMON_QEMU_ARGS="
$PRANAOS_EXTRA_QEMU_ARGS
-s -m $PRANAOS_RAM_SIZE
-cpu $PRANAOS_QEMU_CPU
-d guest_errors
-smp 2
-device VGA,vgamem_mb=64
-drive file=${PRANAOS_DISK_IMAGE},format=raw,index=0,media=disk
-device ich9-ahci
-usb
-device virtio-serial
-chardev stdio,id=stdout,mux=on
-device virtconsole,chardev=stdout
-device isa-debugcon,chardev=stdout
-device virtio-rng-pci
-soundhw pcspk
-device sb16
"

[ -z "$PRANAOS_COMMON_QEMU_Q35_ARGS" ] && PRANAOS_COMMON_QEMU_Q35_ARGS="
$PRANAOS_EXTRA_QEMU_ARGS
-s -m $PRANAOS_RAM_SIZE
-cpu $PRANAOS_QEMU_CPU
-machine q35
-d guest_errors
-smp 2
-device VGA,vgamem_mb=64
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
    # Meta/run.sh b: bochs
    [ -z "$PRANAOS_BOCHSRC" ] && {
        # Make sure that PRANAOS_SOURCE_DIR is set and not empty
        [ -z "$PRANAOS_SOURCE_DIR" ] && die 'PRANAOS_SOURCE_DIR not set or empty'
        PRANAOS_BOCHSRC="$PRANAOS_SOURCE_DIR/Meta/bochsrc"
    }
    "$PRANAOS_BOCHS_BIN" -q -f "$PRANAOS_BOCHSRC"
elif [ "$PRANAOS_RUN" = "qn" ]; then
    # Meta/run.sh qn: qemu without network
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        -device e1000 \
        -kernel Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
elif [ "$PRANAOS_RUN" = "qtap" ]; then
    # Meta/run.sh qtap: qemu with tap
    sudo ip tuntap del dev tap0 mode tap || true
    sudo ip tuntap add dev tap0 mode tap user "$(id -u)"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev tap,ifname=tap0,id=br0 \
        -device e1000,netdev=br0 \
        -kernel Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
    sudo ip tuntap del dev tap0 mode tap
elif [ "$PRANAOS_RUN" = "qgrub" ]; then
    # Meta/run.sh qgrub: qemu with grub
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23 \
        -device e1000,netdev=breh
elif [ "$PRANAOS_RUN" = "q35_cmd" ]; then
    # Meta/run.sh q35_cmd: qemu (q35 chipset) with PRANAOSOS with custom commandline
    shift
    PRANAOS_KERNEL_CMDLINE="$*"
    echo "Starting PRANAOSOS, Commandline: ${PRANAOS_KERNEL_CMDLINE}"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_Q35_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23 \
        -device e1000,netdev=breh \
        -kernel Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
elif [ "$PRANAOS_RUN" = "qcmd" ]; then
    # Meta/run.sh qcmd: qemu with PRANAOSOS with custom commandline
    shift
    PRANAOS_KERNEL_CMDLINE="$*"
    echo "Starting PRANAOSOS, Commandline: ${PRANAOS_KERNEL_CMDLINE}"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23 \
        -device e1000,netdev=breh \
        -kernel Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
elif [ "$PRANAOS_RUN" = "ci" ]; then
    # Meta/run.sh ci: qemu in text mode
    echo "Running QEMU in CI"
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_EXTRA_QEMU_ARGS \
        -s -m $PRANAOS_RAM_SIZE \
        -cpu $PRANAOS_QEMU_CPU \
        -d guest_errors \
        -smp 2 \
        -drive file=${PRANAOS_DISK_IMAGE},format=raw,index=0,media=disk \
        -device ich9-ahci \
        -nographic \
        -display none \
        -debugcon file:debug.log \
        -kernel Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
else
    # Meta/run.sh: qemu with user networking
    "$PRANAOS_QEMU_BIN" \
        $PRANAOS_COMMON_QEMU_ARGS \
        $PRANAOS_VIRT_TECH_ARG \
        $PRANAOS_PACKET_LOGGING_ARG \
        -netdev user,id=breh,hostfwd=tcp:127.0.0.1:8888-10.0.2.15:8888,hostfwd=tcp:127.0.0.1:8823-10.0.2.15:23,hostfwd=tcp:127.0.0.1:8000-10.0.2.15:8000,hostfwd=tcp:127.0.0.1:2222-10.0.2.15:22 \
        -device e1000,netdev=breh \
        -kernel Kernel/Kernel \
        -append "${PRANAOS_KERNEL_CMDLINE}"
fi
