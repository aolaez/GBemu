cmake_minimum_required(VERSION 3.10)

# Include your project's headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find Check library using pkg-config (works on Ubuntu/Debian)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)

# Include directories from Check
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

# List your test sources
set(TEST_SOURCES
    check_gbe.c
)

# Create the test executable
add_executable(check_gbe ${TEST_SOURCES})

# Link with your emulator library and Check
target_link_libraries(check_gbe
    emu                  # your main emulator code
    ${CHECK_LIBRARIES}   # Check unit test library
    m                    # math library
    rt                   # realtime library
    pthread              # threads
    subunit              # optional, if you use it
)

# Make sure test executable can see your headers
target_include_directories(check_gbe PRIVATE ${PROJECT_SOURCE_DIR}/include)

