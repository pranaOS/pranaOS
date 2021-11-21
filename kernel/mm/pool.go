package mm

import (
	"unsafe"

	"github.com/pranaOS/pranaOS/kernel/sys"
)

type memblk struct {
	next uintptr
}

type Pool struct {
	size uintptr
	head uintptr
}

func PoolInit(p *Pool, size uintptr) {
	const align = sys.PtrSize - 1
	size = (size + align) &^ align
	p.size = size
}

func (p *Pool) Free(ptr uintptr) {
	v := (*memblk)(unsafe.Pointer(ptr))
	v.next = p.head
	p.head = ptr
}
