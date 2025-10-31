cmake_minimum_required(VERSION 3.22)
project(GBemu C)

set(CMAKE_C_STANDARD 99)

# ------------------------
# Sources and headers
# ------------------------
set(MAIN_SOURCES
    src/main.c
)

# Grab all other source files in src/
file(GLOB OTHER_SOURCES "src/*.c")
list(REMOVE_ITEM OTHER_SOURCES ${MAIN_SOURCES})  # remove main.c from OTHER_SOURCES

# Grab all headers
file(GLOB HEADERS "include/*.h")

# ------------------------
# Executable
# ------------------------
add_executable(gbemu ${MAIN_SOURCES} ${OTHER_SOURCES} ${HEADERS})
target_include_directories(gbemu PUBLIC include)

# ------------------------
# SDL2
# ------------------------
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(gbemu PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(gbemu ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

message(STATUS "SDL Libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL TTF Libraries: ${SDL2_TTF_LIBRARIES}")

# ------------------------
# Optional: copy font after build
# ------------------------
add_custom_command(
    TARGET gbemu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/NotoSansMono-Medium.ttf
            $<TARGET_FILE_DIR:gbemu>
)

# ------------------------
# Install rules (optional)
# ------------------------
install(TARGETS gbemu
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

