cmake_minimum_required(VERSION 3.5)
project(opsd)

set(TARGET "opsd")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

string(TOLOWER "$ENV{COMPILE_FALG}" CMAKE_BUILD_TYPE_LOWER)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
    add_definitions(-D_DEBUG_SIMULATE_)
ENDIF()

execute_process(COMMAND protoc-c --c_out=../gpb --proto_path=../gpb ../gpb/gpb_control_loop.proto)

include_directories("../../libs/util-lib/")
include_directories("../../libs/dsc-lib/include")
include_directories("../../libs/mgmt-lib/liboplkipc")
include_directories("../../libs/mgmt-lib/liboplknet/")
include_directories("../../libs/mgmt-lib/liboplkalm/")
include_directories("../../libs/hal/api_lib/inc")
include_directories($ENV{INCLUDE_DIR})

aux_source_directory(./ SRC_CUR_DIR)

LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable(${TARGET} ${SRC_CUR_DIR} ${SRC_GPB_DIR})

target_link_libraries(${TARGET} halapi oplkutil oplkdsc oplkipc oplkalm oplknet sysrepo protobuf-c pthread m crypt)
