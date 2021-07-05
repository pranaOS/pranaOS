#!/usr/bin/env python3

# imports
import sys
import subprocess

disk_image = sys.argv[1]
qemu_command = [
    "qemu-system-x86_64",
    "-hda", disk_image,
    "-serial", "mon:stdio",
    "-nographic",
    "-rtc", "base=localtime",
    "-no-reboot",
    "-m", "1G"]

qemu_proc = subprocess.Popen(
    qemu_command,
    stdout=subprocess.PIPE,
    stdin=subprocess.PIPE)

qemu_stdin = qemu_proc.stdin
qemu_stdout = qemu_proc.stdout


