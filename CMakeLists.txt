cmake_minimum_required(VERSION 3.16)
project(main LANGUAGES CXX)

# Compiler settings similar to your Makefile
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic)

# Collect sources recursively under source/
# CONFIGURE_DEPENDS makes CMake re-scan on new files after re-configure
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     ${CMAKE_SOURCE_DIR}/source/*.cpp)

# Create the executable
add_executable(main ${SOURCES})

# Let your code include headers like #include "foo.hpp" from source/
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/source)

# Find and link SFML (graphics, window, system like your Makefile)
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)

# Optional: a `run` target to mimic `make run`
add_custom_target(run
  COMMAND $<TARGET_FILE:main>
  DEPENDS main
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
