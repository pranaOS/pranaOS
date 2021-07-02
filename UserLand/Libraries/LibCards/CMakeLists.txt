set(SOURCES
    Card.cpp
    CardStack.cpp
)

pranaos_lib(LibCards cards)
target_link_libraries(LibCards LibC LibCore)
