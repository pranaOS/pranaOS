
set(SOURCES
  validation.cpp
)

pranaos_lib(libdevicetree devicetree)
target_link_libraries(libdevicetree libc libcore)
