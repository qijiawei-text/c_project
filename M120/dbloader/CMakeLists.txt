cmake_minimum_required(VERSION 3.5)
project(dbloader)

set(TARGET "dbloader")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

### debug
string(TOLOWER "$ENV{COMPILE_FALG}" CMAKE_BUILD_TYPE_LOWER)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
    add_definitions(-D_DEBUG_SIMULATE_)
ENDIF()

### product
string(TOLOWER "$ENV{PRJ_NAME}" CMAKE_BUILD_PRJ_NAME)
IF(CMAKE_BUILD_PRJ_NAME MATCHES nr1004)
    add_definitions(-D_NR1004_)
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES olsp)
    add_definitions(-D__OLSP__)
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES hcf)
    add_definitions(-D__HCF__)
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES ocs)
	add_definitions(-D_OCS_)
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES otdr)
	add_definitions(-D__OTDR__)
ENDIF()

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_BUILD_PRJ_NAME)

include_directories("../../libs/util-lib/")
include_directories("../../libs/mgmt-lib/liboplkpersist")
include_directories("../../libs/mgmt-lib/liboplktrans")
include_directories("../../libs/hal/api_lib/inc")
include_directories($ENV{INCLUDE_DIR})

aux_source_directory(./ SRC_CUR_DIR)

LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable(${TARGET} ${SRC_CUR_DIR})

target_link_libraries(${TARGET} halapi oplkutil oplkpersist oplktrans sysrepo yang pthread)
