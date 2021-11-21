package mm

import (
	"unsafe"
)

const (
	PGSIZE = 4 << 10
	MEMSTART = 100 << 20
	DEFAULT_MEMTOP = 256 << 20
	VMSTART = 1 << 30
	PTR_P = 0x001
	PTE_W = 0x002
	PTE_U = 0x004

	_ENTRY_NUMBER = PGSIZE / sys.PtrSize
)

var (
	memtop uinptr

	kmm = kmmt{voffset: VMSTART}
	vmm vmmt
)