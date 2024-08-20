cmake_minimum_required(VERSION 3.5)

project(ieeefloat32_test)
set(TARGET "ieeefloat32_test")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")


### product

aux_source_directory(./ SRC_CUR_DIR)

add_executable(${TARGET} ${SRC_CUR_DIR})

target_link_libraries(${TARGET})
