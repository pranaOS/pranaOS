add_library(libcryptsha2 OBJECT ../libCcypto/hash/sha2.cpp)
set_target_properties(libcryptsha2 PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(libcryptsha2 PROPERTIES VISIBILITY_INLINES_HIDDEN ON)

set(SOURCES
    crypt.cpp
)

pranaos_libc(libcrypt crypt)
target_link_libraries(libcrypt libc libcryptsha2)
