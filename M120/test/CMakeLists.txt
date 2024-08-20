project(app-test)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O3 -Wall -Werror")

string(TOLOWER "$ENV{ILA_PRODUCT}" PRODUCT_BUILD_TYPE)
message("PRODUCT_BUILD_TYPE = ${PRODUCT_BUILD_TYPE}")

set(SHASTA_BIGSUR_PROJECT_DIR  ../..)
include_directories("${SHASTA_BIGSUR_PROJECT_DIR}/apps_pub/sysrepo/inc")
#include_directories("${SHASTA_BIGSUR_PROJECT_DIR}/libs/dev-lib/API_lib/api")
include_directories("${SHASTA_BIGSUR_PROJECT_DIR}/libs/dsc-lib/include")
include_directories("${SHASTA_BIGSUR_PROJECT_DIR}/libs/util-lib")

LINK_DIRECTORIES("/usr/local/lib")
add_subdirectory(unit_test)
add_executable(oplk_of_config oplk_of_config.c)
add_executable(oplk_device oplk_device.c)
add_executable(oplk_agent oplk_agent.c)
add_executable(oplk_agent_alarm oplk_agent_alarm.c)

target_link_libraries(oplk_of_config oplkdsc oplkutil sysrepo crypt)
target_link_libraries(oplk_device oplkdsc oplkutil sysrepo crypt)
target_link_libraries(oplk_agent oplkdsc oplkutil sysrepo crypt)
target_link_libraries(oplk_agent_alarm oplkdsc oplkutil sysrepo crypt)
