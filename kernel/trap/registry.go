package trap

var trapHandlers = [256]TrapHandler{}

type TrapHandler func()

func Handler(no int) TrapHandler {
	return trapHandlers[no]
}

func Register(idx int, handler func()) {
	trapHandlers[idx] = handler
}