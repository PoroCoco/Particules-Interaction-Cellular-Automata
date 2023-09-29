cmake_minimum_required(VERSION 3.0)

project("Automata" C)

set(CMAKE_C_FLAGS "-std=c99 -Wall -pthread -Wextra -Wunused-parameter -Wstrict-prototypes")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -fsanitize=address,undefined")    # use CMake option: -DCMAKE_BUILD_TYPE=DEBUG
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG ")   # use CMake option: -DCMAKE_BUILD_TYPE=RELEASE


include_directories(raylib-4.5.0/src/)
link_directories(raylib-4.5.0/src/)

add_executable(main main.c utils.c particules.c graphics.c)
target_link_libraries(main raylib m pthread dl rt X11 GL  )

