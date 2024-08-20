project(halapi)
cmake_minimum_required(VERSION 3.5)
set(TARGET "halapi")

### Compiler
### X86 Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

### Powerpc Compiler
#SET(CMAKE_CXX_FLAGS "-O3 -std=c++11 -Wall -pthread -D_GNU_SOURCE -D__USE_XOPEN  -m64 -mhard-float -mcpu=e5500 --sysroot=/opt/windriver/wrlinux-small/8.0-fsl-t1xxx/sysroots/ppc64e5500-wrs-linux")
#SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER    "powerpc64-wrs-linux-gcc")
#SET(CMAKE_CXX_COMPILER  "powerpc64-wrs-linux-g++")

### Debug
string(TOLOWER "$ENV{COMPILE_FLAG}" CMAKE_BUILD_TYPE_LOWER)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
    add_definitions(-D_DEBUG_SIMULATE_)
ENDIF()

### product
string(TOLOWER "$ENV{PRJ_NAME}" CMAKE_BUILD_PRJ_NAME)
IF(CMAKE_BUILD_PRJ_NAME MATCHES nr1004)
    add_definitions(-D__NR1004__)   
ENDIF()


IF(CMAKE_BUILD_PRJ_NAME MATCHES olsp) 
	add_definitions(-D__OLSP__) 
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES hcf) 
	add_definitions(-D__HCF__) 
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES ocs)
	add_definitions(-D__OCS__)
ENDIF()

IF(CMAKE_BUILD_PRJ_NAME MATCHES otdr)
	add_definitions(-D__OTDR__)
ENDIF()

### API Layer
include_directories("./inc")
include_directories("./api")
### Dev Layer
include_directories("../dev/shm")
include_directories("../dev/drv")
include_directories("../dev/srvl")
include_directories("../dev/sdv")
include_directories("../common")
include_directories("../../util-lib")

aux_source_directory(./api              SRC_DIR_API)
aux_source_directory(../dev/shm         SRC_DIR_DEV_SHM)
aux_source_directory(../common          SRC_DIR_COMMON)

add_library(${TARGET} SHARED  ${SRC_DIR_API}
	                            ${SRC_DIR_DEV_SHM}
	                            ${SRC_DIR_COMMON})


LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable (api_main api_main.cpp)
target_link_libraries (api_main halapi oplkutil pthread)

add_executable (api_c_main api_c_main.c)
target_link_libraries (api_c_main halapi oplkutil pthread)
