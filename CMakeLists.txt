cmake_minimum_required(VERSION 3.8)
project(BreakOut)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# Find SDL2 libraries
set(SDL2_PATH "C:/SDL2-2.30.2/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)

# Find SDL GFX
find_package(SDL2_gfx REQUIRED)

# Set the include directories
include_directories(${SDL2_INCLUDE_DIR})

# Set the source files
set(SOURCES
        src/main.cpp
        src/game.cpp
        src/ball.cpp
)

# Set the header files
set(HEADERS
        include/game.h
        include/ball.h
        include/config.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against SDL2 libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})



