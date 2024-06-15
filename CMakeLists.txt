cmake_minimum_required(VERSION 3.28)
project(Chip8_Emulator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
find_package(SDL2 REQUIRED)
include_directories( ${SDL2_INCLUDE_DIR} )

add_executable(Chip8_Emulator src/main.cpp
        src/Chip8.cpp
        src/includes/Chip8.h
        src/Platform.cpp
        src/includes/Platform.h
)

target_link_libraries(Chip8_Emulator ${SDL2_LIBRARY} )