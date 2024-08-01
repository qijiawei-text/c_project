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

    //解析数据
    cJSON* item = NULL;
    char* v_str = NULL;
    double v_double = 0.0;
    int v_int = 0;
    bool v_bool = false;


    //解析： name : 小明
    item = cJSON_GetObjectItem(root, "name");
    if (item != NULL)   {
        //方法一
        if (item->type == cJSON_String){
            v_str = cJSON_Print(item);
            printf("name = %s\n", v_str);
            free(v_str);
            v_str = NULL;
        }

        //方法二
        if (item->type == cJSON_String){
            v_str = item->valuestring;
            printf("name = %s\n", v_str);
        }
    }


    //解析： age : 23
    item = cJSON_GetObjectItem(root, "age");
    if (item != NULL)   {

        //方法二
        if (item->type == cJSON_Number){
            v_int = item->valueint;
            printf("age = %d\n", v_int);
        }
    }

    //解析: vip : true
    item = cJSON_GetObjectItem(root, "vip");
    if (item != NULL) {
        if ((item->type == cJSON_True) || (item->type == cJSON_False)){
            v_str = cJSON_Print(item);
            printf("vip = %s\n", v_str);
            free(v_str);
            v_str = NULL;
        }
    }

    //解析 address ：null
    item = cJSON_GetObjectItem(root, "address");
    if (item != NULL) {
        if (item->type == cJSON_NULL){
            v_str = cJSON_Print(item);
            printf("address = %s\n", v_str);
            free(v_str);
            v_str = NULL;
        }
    }

    /*解析 ："interest":	{
		            "sport":	["篮球", "足球"],
		            "book":	["unix高级环境编程", "c prime plus"]
	}*/
    item = cJSON_GetObjectItem(root, "interest");
    if(item != NULL) {
        cJSON* val = NULL;
        cJSON* arr = NULL;
        //解析sport数组
        val = cJSON_GetObjectItem(item, "sport");
        if (val != NULL){
            int size = cJSON_GetArraySize(val);
            for(int i = 0; i< size; i++){
                arr = cJSON_GetArrayItem(val, i);
                if ((arr != NULL) && (arr->type == cJSON_String)) {
                    v_str = cJSON_Print(arr);
                    printf("sport = %s\n", v_str);
                    free(v_str);
                    v_str = NULL;
                }
            }
        }
        val = NULL;
        val = cJSON_GetObjectItem(item, "book");
        if (val != NULL){
            int size = cJSON_GetArraySize(val);
            for(int i = 0; i< size; i++){
                arr = cJSON_GetArrayItem(val, i);
                if ((arr != NULL) && (arr->type == cJSON_String)) {
                    v_str = cJSON_Print(arr);
                    printf("book = %s\n", v_str);
                    free(v_str);
                    v_str = NULL;
                }

            }
        }
        free(arr);
        free(val);

    }

    //解析"color":	["black", "white"],
    item = cJSON_GetObjectItem(root, "color");
    if (item != NULL){
            cJSON* arr = NULL;
            int size = cJSON_GetArraySize(item);
            for(int i = 0; i< size; i++){
                arr = cJSON_GetArrayItem(item, i);
                if ((arr != NULL) && (arr->type == cJSON_String)) {
                    v_str = cJSON_Print(arr);
                    printf("color = %s\n", v_str);
                    free(v_str);
                    v_str = NULL;
                }
            }
            free(arr);
    }

    /* 解析："like":  [{
		    	"game":	"塞尔达旷野之息",
		    	"price":	256.8
		    }, {
		    	"game":	"Eding Ring",
		    	"price":	299
		    }],*/
    item = cJSON_GetObjectItem(root, "like");
        if (item != NULL){
            cJSON* val = NULL;
            cJSON* arr = NULL;
            int size = cJSON_GetArraySize(item);
            for(int i = 0; i< size; i++){
                val = cJSON_GetArrayItem(item, i);
                if((val != NULL) && (val->type == cJSON_Object)){
                    arr = cJSON_GetObjectItem(val, "game");
                    if ((arr != NULL) && (arr->type == cJSON_String)){
                        v_str = cJSON_Print(arr);
                        printf("book = %s\n", v_str);
                        free(v_str);
                        v_str = NULL;
                    }
                    arr = cJSON_GetObjectItem(val, "price");
                    if ((arr != NULL) && (arr->type == cJSON_Number)){
                        v_double = arr->valuedouble;
                        printf("book = %.2f\n", v_double);
                    }
                }
            }
            free(val);
            free(arr);
        }

    /*解析： "eduaction":	[["小学", "初中"], ["高中", "大学"]],*/
    item = cJSON_GetObjectItem(root, "education");
    if (item != NULL){
        printf("-----------");
        cJSON* val = NULL;
        cJSON* arr = NULL;
        int size = cJSON_GetArraySize(item);
        printf("size: %d", size);
        for(int i = 0; i< size; i++){
            val = cJSON_GetArrayItem(item, i);
            if((val != NULL) && (val->type == cJSON_Array)){
                int arrsize = cJSON_GetArraySize(val);
                for(int j =0; j< arrsize; j++){
                    arr = cJSON_GetArrayItem(val,j);
                    if((arr != NULL) && (arr->type == cJSON_String)){
                        v_str = cJSON_Print(arr);
                        printf("education = %s\n", v_str);
                        free(v_str);
                        v_str = NULL;
                    }
                }
            }
        }
        free(val);
        free(arr);
    }
    return 0;
}