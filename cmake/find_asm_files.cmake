macro(find_asm_source_files OutValue Directory)
    file(GLOB ${OutValue} LIST_DIRECTORIES false ${Directory}/*.[sS])
    message(STATUS "found asm files: ${${OutValue}}")
endmacro()