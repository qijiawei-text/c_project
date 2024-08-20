cmake_minimum_required(VERSION 3.5)
project(util-lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall")

include_directories("./")

aux_source_directory(./ SRC_CUR_DIR)

add_library(oplkutil SHARED ${SRC_CUR_DIR})

target_link_libraries (oplkutil -lm)
