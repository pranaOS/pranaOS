package pic

import "github.com/pranaOS/pranaOS/kernel/sys"

const (
	PIC1_CMD  = 0x20
	PIC1_DATA = PIC1_CMD + 1
	PIC2_CMD  = 0xA0
	PIC2_DATA = PIC2_CMD + 1

	ICW4_8086 = 0x01
	ICW4_AUTO = 0x02

	IRQ_BASE = 0x20

	LINE_TIMER = 0
	LINE_KBD   = 1
	LINE_COM1  = 4
	LINE_COM2  = 3
	LINE_MOUSE = 12
)

func Init() {

	sys.Outb(PIC1_CMD, 0x11)
	sys.Outb(PIC2_CMD, 0x11)

	sys.Outb(PIC1_DATA, IRQ_BASE)
	sys.Outb(PIC2_DATA, IRQ_BASE+8)

	sys.Outb(PIC1_DATA, 0x4)
	sys.Outb(PIC2_DATA, 0x2)

	sys.Outb(PIC1_DATA, ICW4_8086)
	sys.Outb(PIC2_DATA, ICW4_8086)

	sys.Outb(PIC1_DATA, 0xff)
	sys.Outb(PIC2_DATA, 0xff)

	EnableIRQ(0x02)
}

func EnableIRQ(line uint16) {
	var port uint16 = PIC1_DATA
	if line >= 8 {
		port = PIC2_DATA
		line -= 8
	}
	sys.Outb(port, byte(sys.Inb(port)&^(1<<line)))
}

func DisableIRQ(line uint16) {
	var port uint16 = PIC1_DATA
	if line >= 8 {
		port = PIC2_DATA
		line -= 8
	}
	sys.Outb(port, byte(sys.Inb(port)|(1<<line)))
}

func EOI(irq uintptr) {
	if irq >= 0x28 {
		sys.Outb(PIC2_CMD, 0x20)
	}
	sys.Outb(PIC1_CMD, 0x20)
}