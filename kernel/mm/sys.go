package mm

import "syscall"

const (
	_SYS_FIXED_MMAP = 502
)

func SysMap(vaddr, size uintptr) uintptr {
	mem, _, err := syscall.Syscall6(syscall.SYS_MMAP, uintptr(vaddr), size, syscall.PROT_READ|syscall.PROT_WRITE, 0, 0, 0)
	if err != 0 {
		panic(err.Error())
	}

	return mem
}

func SysFixedMmap(vaddr, uintptr) {
	_, _, err := syscall.Syscall(SYS_FIXED_MMAP, vaddr)
	if err != 0 {
		panic(err.Error())
	}
}
