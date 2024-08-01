#include"cJSON.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_NAME "./test_2.json"

int main(void)
{
    //打开文件
    FILE *file =NULL;
    file  = fopen(FILE_NAME, "r");
    if (file == NULL){
        printf("open file fail!\n");
    }

    //获取文件信息
    struct stat statbuf;
    stat(FILE_NAME, &statbuf);
    int fileSize = statbuf.st_size;
    printf("文件大小： %d\n", fileSize);

    //分配内存
    char* json_Str = (char* )malloc(sizeof(char)*(fileSize + 1));
    memset(json_Str, 0, fileSize + 1);

    //读取文件中json字符串
    int size = fread(json_Str, sizeof(char), fileSize, file);
    if(size == 0){
        printf("read file fail!\n");
        fclose(file);
    }
    printf("%s\n", json_Str);
    fclose(file);

    //将json字符串数据初始化为cJSON指针
    cJSON* root = cJSON_Parse(json_Str);
    if (root == NULL){
        const char* err_prt = cJSON_GetErrorPtr();
        if (err_prt != NULL){
            fprintf(stderr, "Error before: %s\n",err_prt);
        }
    }
    free(json_Str);


    cJSON* item = NULL;
    //修改： name -> 漆嘉伟_update
    cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("漆嘉伟_update"));

    //修改： age -> 33
    cJSON_ReplaceItemInObject(root, "age", cJSON_CreateNumber(33));

    //修改： vip -> false
    cJSON_ReplaceItemInObject(root, "vip", cJSON_CreateFalse());

    /*修改： "interest":	{
		"sport":	["乒乓球", "足球"],
		"book":	["unix高级环境编程", "嵌入式c语言自我修养"]
	},*/
    item = cJSON_GetObjectItem(root, "interest");
    if(item != NULL){
        cJSON* var = NULL;
        var = cJSON_GetObjectItem(item, "sport");
        cJSON_ReplaceItemInArray(var, 0 ,cJSON_CreateString("乒乓球"));

        var = cJSON_GetObjectItem(item, "book");
        cJSON_ReplaceItemInArray(var, 1 ,cJSON_CreateString("嵌入式c语言自我修养"));
    }

    /*修改： "like":	[{
			"game":	"塞尔达旷野之息",
			"price":	256.8
		}, {
			"game":	"艾尔登法环：黄金树之影",
			"price":   499	
		}],*/
    item = cJSON_GetObjectItem(root, "like");
    if (item != NULL){
        cJSON* var = NULL;
        var =cJSON_GetArrayItem(item, 1);
        cJSON_ReplaceItemInObject(var, "game", cJSON_CreateString("艾尔登法环：黄金树之影"));
        cJSON_ReplaceItemInObject(var, "price", cJSON_CreateNumber(499.0));

    }

    /*修改： "lagngues":	{
		"serialOne":	{
			"language":	"汉语",
			"grade":	10
		},
		"serialTwo":	{
			"language":	"日语",
			"grade":	8
		}
	},*/
    item = cJSON_GetObjectItem(root, "lagngues");
    if (item != NULL){
        cJSON* var = NULL;
        var =cJSON_GetObjectItem(item, "serialTwo");
        cJSON_ReplaceItemInObject(var, "language", cJSON_CreateString("日语"));
        cJSON_ReplaceItemInObject(var, "grade", cJSON_CreateNumber(8));
    }

    // 打开文件
    file = fopen(FILE_NAME, "w");
    if (file == NULL) {
	    printf("Open file fail！\n");
	    // 释放指针内存
	    cJSON_Delete(root);
        return 0;
    }

    char *cjValue = cJSON_Print(root);
    // 写入文件
    int ret = fwrite(cjValue, sizeof(char), strlen(cjValue), file);
    if (ret == 0) {
    	printf("写入文件失败！\n");
    }

    fclose(file);
    free(cjValue);
    return 0;
}