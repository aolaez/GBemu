# Grab all source files in lib/
file(GLOB sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/*.c")

# Grab all headers
file(GLOB headers CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

# Define the static library
add_library(emu STATIC ${sources} ${headers})

# Include directories
target_include_directories(emu PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Find SDL2 and SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include SDL headers
target_include_directories(emu PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link SDL libraries
# On Ubuntu, SDL2_ttf may not populate SDL2_TTF_LIBRARIES automatically,
# so link it explicitly if needed
target_link_libraries(emu PUBLIC
    ${SDL2_LIBRARIES}
    /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so
)
