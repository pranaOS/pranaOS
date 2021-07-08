LIBS += C

C_NAME = c


C_CXXFLAGS = \
	-fno-tree-loop-distribute-patterns \
	-fno-rtti \
	-fno-exceptions \
	-DDISABLE_LOGGER

C_CFLAGS = -fno-tree-loop-distribute-patterns