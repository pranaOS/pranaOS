package mm

import "syscall"

const (
	_SYS_FIXED_MMAP = 502
)

func SysMmap(vaddr, size uintptr) uintptr {
	mem, _, err := syscall.Syscall6(syscall.SYS_MMAP, uintptr(vaddr), size, syscall.PROT_READ|syscall.PROT_WRITE, 0, 0, 0)
	if err != 0 {
		panic(err.Error())
	}
	return mem
}

func SysFixedMmap(vaddr, paddr, size uintptr) {
	_, _, err := syscall.Syscall(_SYS_FIXED_MMAP, vaddr, paddr, size)
	if err != 0 {
		panic(err.Error())
	}
}
