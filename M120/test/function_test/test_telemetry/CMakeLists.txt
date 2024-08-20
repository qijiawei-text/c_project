cmake_minimum_required(VERSION 3.5)


set(TARGET "telemetry_server")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

string(TOLOWER "$ENV{COMPILE_FALG}" CMAKE_BUILD_TYPE_LOWER)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
    add_definitions(-D_DEBUG_SIMULATE_)
ENDIF()

execute_process(COMMAND protoc-c --c_out=../gpb --proto_path=../gpb ../gpb/optical_top.proto ../gpb/optical_port_unmanaged.proto ../gpb/optical_port_managed.proto ../gpb/optical_port_info.proto ../gpb/optical_edfa.proto ../gpb/optical_voa.proto)

include_directories("../../../../libs/util-lib/")
include_directories("../../../../libs/dsc-lib/include")
include_directories("../../../../libs/mgmt-lib/liboplknet/")
include_directories("../../../../libs/mgmt-lib/liboplkalm/")
include_directories("../../../../libs/pluggable-dev-lib/API_lib/api")
include_directories("../../../../libs/pluggable-dev-lib/SCM_lib/include")
include_directories("../../../../libs/pluggable-dev-lib/SCM_lib/api")
include_directories("../../../../libs/mgmt-lib/liboplksyslog/")


include_directories("./gpb")

aux_source_directory(./ SRC_CUR_DIR)

aux_source_directory(./gpb SRC_GPB_DIR)

LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable(${TARGET} ${SRC_CUR_DIR} ${SRC_GPB_DIR})

target_link_libraries(${TARGET} oplkutil oplkdsc oplkalm oplknet sysrepo protobuf-c pthread m crypt)
