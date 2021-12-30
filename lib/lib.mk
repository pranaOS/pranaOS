LIBS += LIBPRANAOS
LIBPRANAOS_NAME = libpranaos

LIBS += LIBC
LIBC_NAME = libc

LIBS += LIBSTDCXX
LIBSTDCXX_NAME = libstdc++

define LIBRARY_TEMPLATE =
$(1)_LIB := $(LIBS_FOLDER)/$($(1)_NAME).a

$(1)_SOURCES += \
		$$(call rwildcard, lib/$($(1)_NAME), *.cpp)

$(1)_HEADERS += \
		$$(call rwildcard, lib/$($(1)_NAME), *.h)

$(1)_OBJECTS := $$(patsubst %.cpp, $(BIN_FOLDER)/%.o, $$($(1)_SOURCES))

TARGETS += $$($(1)_LIB)
# this line should be removed but some libaries aren't behaving with the new setup
KERNEL_OBJECTS += $$($(1)_OBJECTS)

$$($(1)_LIB): $$($(1)_OBJECTS)
	@mkdir -p $$(@D)
	@echo [ creating library archive $@ for $$($(1)_NAME) ] $(AR)
	@$(AR) -rsc $$@ $$^

$$(BIN_FOLDER)/lib/$($(1)_NAME)/%.o: lib/$$($(1)_NAME)/%.cpp $($(1)_HEADERS)
	@mkdir -p $$(@D)
	@echo [ compiling target $$@ ] C++
	@$(CXX) -c $$< -o $$@ $(CFLAGS)

endef

$(foreach lib, $(LIBS), $(eval $(call LIBRARY_TEMPLATE,$(lib))))