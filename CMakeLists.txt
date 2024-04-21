cmake_minimum_required(VERSION 3.8)
project(GameProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 libraries

find_package(SDL2 REQUIRED)


# Set the include directories
include_directories(${SDL2_INCLUDE_DIRS})

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
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link against SDL2 libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})