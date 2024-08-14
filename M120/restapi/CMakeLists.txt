cmake_minimum_required(VERSION 3.5)
project(restapi)
set(TARGET "restapi")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

#mongoose define
add_definitions(-DMG_ENABLE_OPENSSL=1)
add_definitions(-DMG_ENABLE_IPV6=1)
add_definitions(-DMG_ENABLE_LINES=1)

#DSC library define
add_definitions(-DOC_SYS_NTP)

### product
string(TOLOWER "$ENV{PRJ_NAME}" CMAKE_BUILD_PRJ_NAME)
IF(CMAKE_BUILD_PRJ_NAME MATCHES olsp) 
    add_definitions(-D__OLSP__) 
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES hcf)
    add_definitions(-D__HCF__) 
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES ocs)
    add_definitions(-D__OCS__)
ENDIF()

include_directories("../../libs/util-lib/")
include_directories("../../libs/dsc-lib/include")
include_directories("../../libs/mgmt-lib/liboplktrans/")
include_directories("../../libs/hal/api_lib/inc")
include_directories($ENV{INCLUDE_DIR})

aux_source_directory(./ SRC_CUR_DIR)
#message(STATUS "SRC_CUR_DIR:  ${SRC_CUR_DIR}") 

LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable(${TARGET} ${SRC_CUR_DIR})

target_link_libraries(${TARGET} oplkdsc oplkutil oplkalm sysrepo sysrepo yang mongoose pthread crypt halapi)
#target_link_libraries(${TARGET} oplkdsc oplkutil sysrepo mongoose ssl crypto pthread crypt)
