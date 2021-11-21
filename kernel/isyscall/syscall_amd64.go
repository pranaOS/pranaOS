package isyscall

import "unsafe"

type trapFrame struct {
	AX, BX, CX, DX    uintptr
	BP, SI, DI, R8    uintptr
	R9, R10, R11, R12 uintptr
	R13, R14, R15     uintptr

	Trapno, Err uintptr

	IP, CS, FLAGS, SP, SS uintptr
}

type Request struct {
	tf   *trapFrame
	Lock uintptr
}

func NewRequest(tf uintptr) Request {
	return Request{
		tf: (*trapFrame)(unsafe.Pointer(tf)),
	}
}

func (t *trapFrame) NO() uintptr {
	return t.AX
}
