# opencal/OpenCAL/examples/cal_life/CMakeLists.txt
cmake_minimum_required(VERSION 3.2.1)

project(Sand_seabed)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(PkgConfig REQUIRED)
pkg_search_module(ALLEGRO5 REQUIRED allegro-5 allegro-5.0)
pkg_search_module(ALLEGRO5_PRIMITIVES REQUIRED allegro_primitives-5 allegro_primitives-5.0)

find_package(OpenCAL 1.0 COMPONENTS OpenCAL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${OPENCAL_INCLUDE_DIR}
                    )
file(GLOB SRC
    "include/*.h"
    "source/*.cpp"
    "source/*.c"
)

#link_directories(${PROJECT_SOURCE_DIR}/OpenCAL/lib)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
  ${ALLEGRO5_LIBRARIES}
  ${ALLEGRO5_PRIMITIVES_LIBRARIES}
  ${OPENCAL_LIBRARIES}
)
