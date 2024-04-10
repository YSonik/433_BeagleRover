# Build the app, using the HAL

#                   headerFiles
include_directories(include)

#                   sourceFiles
file(GLOB MY_SOURCES "src/*.c")

#              binaryName
add_executable(run_rover ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(run_rover LINK_PRIVATE hal)

# PTHREAD support
find_package(Threads REQUIRED)
target_link_libraries(run_rover LINK_PRIVATE Threads::Threads)


# Copy executable to final location
add_custom_command(TARGET run_rover POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy
     "$<TARGET_FILE:run_rover>"
     "$(HOME)/cmpt433/public/myApps"
  COMMENT "Copying ARM executable to public NFS directory")
