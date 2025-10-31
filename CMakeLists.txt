cmake_minimum_required(VERSION 3.22)
project(GBemu C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

# -----------------------------------------------------------------------------
# Find SDL2 and SDL2_ttf
# -----------------------------------------------------------------------------
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Make these globally available to subdirs
set(SDL2_LIBS SDL2::SDL2 SDL2::SDL2_ttf)
set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# -----------------------------------------------------------------------------
# Find Check (unit testing framework)
# -----------------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

# -----------------------------------------------------------------------------
# Subdirectories
# -----------------------------------------------------------------------------
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)

# -----------------------------------------------------------------------------
# Enable tests (for ctest)
# -----------------------------------------------------------------------------
enable_testing()
add_test(NAME check_gbe COMMAND check_gbe)
