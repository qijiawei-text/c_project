cmake_minimum_required(VERSION 3.5)

set(TARGET "test_persistd")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

string(TOLOWER "$ENV{COMPILE_FALG}" CMAKE_BUILD_TYPE_LOWER)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
    add_definitions(-D_DEBUG_SIMULATE_)
ENDIF()

include_directories("../../../../libs/util-lib/")
include_directories("../../../../libs/mgmt-lib/liboplkipc")


aux_source_directory(./ SRC_CUR_DIR)

LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable(${TARGET} ${SRC_CUR_DIR})

target_link_libraries(${TARGET} oplkutil oplkipc sysrepo pthread m)
