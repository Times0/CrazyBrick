set(SDL2_IMAGE_PATH "C:/SDL2_image-2.8.2/x86_64-w64-mingw32")  # Set this to the path where SDL2_image is installed

find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
          HINTS
          ${SDL2_IMAGE_PATH}
          PATH_SUFFIXES include/SDL2 include)

find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image
             HINTS
             ${SDL2_IMAGE_PATH}
             PATH_SUFFIXES lib64 lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_image DEFAULT_MSG SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR)

if(SDL2_IMAGE_FOUND)
    set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})
    set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})
else()
    set(SDL2_IMAGE_LIBRARIES)
    set(SDL2_IMAGE_INCLUDE_DIRS)
endif()

mark_as_advanced(SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR)