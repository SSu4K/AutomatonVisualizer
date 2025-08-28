cmake_minimum_required(VERSION 3.16)
project(main LANGUAGES CXX)

# ================================
# Compiler setup
# ================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect sources recursively under source/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     ${CMAKE_SOURCE_DIR}/source/*.cpp)

# ================================
# Create the executable
# ================================
add_executable(main ${SOURCES})

# Add warning flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(main PRIVATE /W4 /permissive-)
endif()

# Let your code include headers like #include "foo.hpp" from source/
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/source)

# ================================
# Dependencies
# ================================
include(FetchContent)

# --- SFML (use system package instead of fetching it) ---
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# --- Dear ImGui ---
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.9   # stable release compatible with SFML2
)
FetchContent_MakeAvailable(imgui)

# --- ImGui-SFML ---
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG v2.6     # last release for SFML2
)
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "Path to Dear ImGui" FORCE)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_FIND_SFML_STATIC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(imgui-sfml)

# ================================
# Linking
# ================================
target_link_libraries(main
    PRIVATE
        sfml-graphics sfml-window sfml-system
        ImGui-SFML::ImGui-SFML
)

# ================================
# Run target (like `make run`)
# ================================
add_custom_target(run
  COMMAND $<TARGET_FILE:main>
  DEPENDS main
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
