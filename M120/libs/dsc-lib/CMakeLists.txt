cmake_minimum_required(VERSION 3.5)
project(dsc-lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

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

add_definitions(-D__OPER__)
add_definitions(-DOC_SYS_NTP)

include_directories("./include")
include_directories("../util-lib")
include_directories("../mgmt-lib/liboplkalm")
include_directories($ENV{INCLUDE_DIR})

aux_source_directory(./ SRC_CUR_DIR)

LINK_DIRECTORIES($ENV{LINK_DIR})
add_library(oplkdsc SHARED ${SRC_CUR_DIR})

target_link_libraries (oplkdsc)
