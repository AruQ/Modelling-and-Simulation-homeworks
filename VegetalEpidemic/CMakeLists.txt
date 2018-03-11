cmake_minimum_required(VERSION 3.2.1)
project(VegetalEpidemic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PkgConfig REQUIRED)
pkg_search_module(ALLEGRO5 REQUIRED allegro-5 allegro-5.0)
pkg_search_module(ALLEGRO5_PRIMITIVES REQUIRED allegro_primitives-5 allegro_primitives-5.0)

file(GLOB SRC
    "include/*.h"
    "*.cpp"
    "*.c"
    "*.h"
)

add_executable(${PROJECT_NAME} ${SRC} )
target_link_libraries(
  ${PROJECT_NAME}
  ${ALLEGRO5_LIBRARIES}
  ${ALLEGRO5_PRIMITIVES_LIBRARIES}
)



