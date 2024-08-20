1. Hal 层的公共头文件，在api_lib\inc目录下
   DevData/DevBoardData/DevEdfaData/DevTdcmData/... 等是定义数据结构的头文件
   DevBoardCApi/DevEdfaCApi/DevTdcmCApi/... 等是定义C函数的头文件

2. csm要调用Hal的函数，需要修改
	a. CMakeLists.txt
	   头文件目录，删除原来pluggable-dev-lib的目录，用hal/api_lib/inc替换
	   include_directories("../../libs/hal/api_lib/inc")
	   库替换，删除devapi，使用devcapi halapi
	   target_link_libraries(${TARGET} devcapi halapi oplkutil oplkdsc oplkipc oplkalm oplkpm oplksyslog sysrepo pthread m pbc crypt)
	   
	b. 需要调用hal函数的c文件中，要包含Hal的头文件
	   #include "DevBoardCApi.h"

	c. 用Hal层提供的函数替换原来的DEV_XXX函数