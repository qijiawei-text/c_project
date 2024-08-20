project(hal)
cmake_minimum_required(VERSION 3.5)
set(TARGET "hal")

### Compiler
### X86 Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Werror")

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

### vevice
string(TOLOWER "$ENV{VEVICE_FLAG}" CMAKE_BUILD_VEVICE_FLAG)

IF(CMAKE_BUILD_VEVICE_FLAG MATCHES enable)
    message("### Support vevice")
    add_definitions(-D_VEVICE_)
ENDIF()

find_package(PkgConfig)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

### Dev Directory
include_directories("../api_lib/inc")
include_directories("./api")
include_directories("./sdv")
include_directories("./shm")
include_directories("./srvl")
include_directories("./drv")
include_directories("./virtual")
include_directories("../common")
include_directories("../../util-lib")

aux_source_directory(.                   SRC_DIR_HAL)
aux_source_directory(../common           SRC_DIR_COMMON)
aux_source_directory(./sdv               SRC_DIR_SDV)
aux_source_directory(./shm               SRC_DIR_SHM)
aux_source_directory(./srvl              SRC_DIR_SRVL)
aux_source_directory(./api               SRC_DIR_DEV_API)
aux_source_directory(./drv               SRC_DIR_DRV)
aux_source_directory(./drv/jtag          SRC_DIR_JTAG)
aux_source_directory(./virtual           SRC_DIR_VIRTUAL)

LINK_DIRECTORIES($ENV{LINK_DIR})
add_executable (${TARGET} ${SRC_DIR_HAL}
                         ${SRC_DIR_COMMON}
                         ${SRC_DIR_SDV}
                         ${SRC_DIR_SHM}
                         ${SRC_DIR_SRVL}
                         ${SRC_DIR_DEV_API}
                         ${SRC_DIR_DRV}
                         ${SRC_DIR_JTAG}
                         ${SRC_DIR_VIRTUAL})

target_link_libraries (${TARGET} oplkutil pthread ft4222 SUSI-4.00 ${JSONCPP_LIBRARIES})
