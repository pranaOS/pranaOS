find_program(UNZIP_TOOL unzip REQUIRED)
if (NOT UNZIP_TOOL)
    message(FATAL_ERROR "Failed to locate unzip on your machine, please install it and re-read the pranaOS build documentation.")
endif()

find_program(TAR_TOOL tar REQUIRED)
if (NOT TAR_TOOL)
    message(FATAL_ERROR "Failed to locate tar on your machine, please install it and re-read the pranaOS build documentation.")
endif()

find_program(GZIP_TOOL gzip REQUIRED)
if (NOT GZIP_TOOL)
    message(FATAL_ERROR "Failed to locate gzip on your machine, please install it and re-read the pranaOS build documentation.")
endif()
