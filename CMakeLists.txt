# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.12)

# Set the project name
project(my_project CXX)

# Set the C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the SDL2 libraries
find_package(SDL2 REQUIRED)

# Add the source files
file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add the executable
add_executable(main ${SRCS})

# Include the include directories
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# Link the libraries
target_link_libraries(main
        ${SDL2_LIBRARIES}
)

# Add the bin and build directories
set(BIN_DIR ${PROJECT_BINARY_DIR}/bin)
set(BUILD_DIR ${PROJECT_BINARY_DIR}/build)
add_custom_target(directories COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR} COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_DIR})
add_dependencies(main directories)
