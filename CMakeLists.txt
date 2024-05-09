cmake_minimum_required(VERSION 3.8)
project(BreakOut)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# Find SDL2 libraries
set(SDL2_PATH "C:/SDL2-2.30.2/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)

# SDl2 ttf
set(SDL2_TTF_PATH "C:/SDL2_ttf-2.22.0/x86_64-w64-mingw32")
find_package(SDL2_ttf REQUIRED)

# Set the include directories
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

# Set the source files
set(SOURCES
        src/main.cpp
        src/game.cpp
        src/ball.cpp
        include/level.h
        src/level.cpp
        include/brick.h
        src/brick.cpp
        include/brick.h
        include/types.h
        include/clock.h
        src/clock.cpp
        src/Powerup.cpp
        include/Powerup.h
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
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})



