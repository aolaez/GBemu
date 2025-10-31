set(MAIN_SOURCES
  main.c
)

add_executable(gbemu ${MAIN_SOURCES})

# Link emu AND SDL2 / SDL2_ttf
target_link_libraries(gbemu emu)

# Include directories
target_include_directories(gbemu PUBLIC ${PROJECT_SOURCE_DIR}/include)

