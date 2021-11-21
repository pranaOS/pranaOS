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

func (p *Pool) grow() {
	start := kmm.alloc()
	end := start + PGSIZE
	for v := start; v+p.size <= end; v += p.size {
		p.Free(v)
	}
}

func (p *Pool) Alloc() uintptr {
	if p.head == 0 {
		p.grow()
	}
	ret := p.head
	h := (*memblk)(unsafe.Pointer(p.head))
	p.head = h.next
	sys.Memclr(ret, int(p.size))
	return ret
}

func (p *Pool) Free(ptr uintptr) {
	v := (*memblk)(unsafe.Pointer(ptr))
	v.next = p.head
	p.head = ptr
}
