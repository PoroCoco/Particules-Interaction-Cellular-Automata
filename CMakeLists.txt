cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)


project ("Particule interation" C)
include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -Wall -pthread -Wextra -Wunused-parameter -Wstrict-prototypes")
set(CMAKE_C_FLAGS_DEBUG "-g --coverage")    # use CMake option: -DCMAKE_BUILD_TYPE=DEBUG
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")   # use CMake option: -DCMAKE_BUILD_TYPE=RELEASE

### SDL 2 ###
find_package(SDL2 REQUIRED)
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries
include_directories(${SDL2_ALL_INC})


file(GLOB SOURCES src/*.c)

add_executable(particule ${SOURCES})





