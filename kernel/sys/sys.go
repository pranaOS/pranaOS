package sys

import "unsafe"

const PtrSize = 4 << (^uintptr(0) >> 63)

const PageSize = 4 << 10

func Outb(port uint16, data byte)

func UnsafeBuffer(p uintptr, n int) []byte {
	return (*[1 << 30]byte)(unsafe.Pointer(p))[:n]
}

//go:nosplit
func Memclr(p uintptr, n int) {
	s := (*(*[1 << 30]byte)(unsafe.Pointer(p)))[:n]
	// the compiler will emit runtime.memclrNoHeapPointers
	for i := range s {
		s[i] = 0
	}
}

func FuncPC(f interface{}) uintptr {
	return **(**uintptr)(unsafe.Pointer((uintptr(unsafe.Pointer(&f)) + PtrSize)))
}

func Fxsave(addr uintptr)

func SetAX(val uintptr)

func CS() uintptr
