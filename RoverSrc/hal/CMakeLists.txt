# CMakeList.txt for HAL
#   Build a library (`hal`) which exposes the header files as "hal/*.h"
#   Use header as: #include "hal/button.h"

#                   headerFiles
include_directories(RoverSrc/hal/include)

#                    sourceFiles
file(GLOB MY_SOURCES "src/*/*.c")
file(GLOB MY_SOURCES_ROOT "src/*.c")

add_library(hal STATIC ${MY_SOURCES} ${MY_SOURCES_ROOT})

target_include_directories(hal PUBLIC include)
