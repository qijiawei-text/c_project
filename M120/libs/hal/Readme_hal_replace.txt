1. Hal ��Ĺ���ͷ�ļ�����api_lib\incĿ¼��
   DevData/DevBoardData/DevEdfaData/DevTdcmData/... ���Ƕ������ݽṹ��ͷ�ļ�
   DevBoardCApi/DevEdfaCApi/DevTdcmCApi/... ���Ƕ���C������ͷ�ļ�

2. csmҪ����Hal�ĺ�������Ҫ�޸�
	a. CMakeLists.txt
	   ͷ�ļ�Ŀ¼��ɾ��ԭ��pluggable-dev-lib��Ŀ¼����hal/api_lib/inc�滻
	   include_directories("../../libs/hal/api_lib/inc")
	   ���滻��ɾ��devapi��ʹ��devcapi halapi
	   target_link_libraries(${TARGET} devcapi halapi oplkutil oplkdsc oplkipc oplkalm oplkpm oplksyslog sysrepo pthread m pbc crypt)
	   
	b. ��Ҫ����hal������c�ļ��У�Ҫ����Hal��ͷ�ļ�
	   #include "DevBoardCApi.h"

	c. ��Hal���ṩ�ĺ����滻ԭ����DEV_XXX����