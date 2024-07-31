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

            {
// 	/*************** 方式一 ***************/
// 	item = cJSON_GetObjectItem(root, "like");
// 	if (item != NULL) {
// 		int size = cJSON_GetArraySize(item);	// 获取的数组大小

// 		for (int i = 0; i < size; i++) {
// 			cJSON *obj = cJSON_GetArrayItem(item, i);		// 获取的数组里的obj
// 			cJSON *val = NULL;

// 			if (obj != NULL && obj->type == cJSON_Object) {	// 判断数字内的元素是不是obj类型
// 				val = cJSON_GetObjectItem(obj, "game");		// 获得obj里的值

// 				if (val != NULL && val->type == cJSON_String) {
// 					v_str = val->valuestring;
// 					printf("game = %s\n", v_str);
// 				}

// 				val = cJSON_GetObjectItem(obj, "price");
// 				if (val != NULL && val->type == cJSON_Number) {
// 					v_double = val->valuedouble;
// 					printf("price = %.2f\n", v_double);
// 				}
// 			}
// 		}
// 	}


// 	/*************** 方式二 ***************/
// 	item = cJSON_GetObjectItem(root, "like");
// 	if (item != NULL) {
// 		cJSON *obj = item->child;	// 获得 { "game": "马里奥", "price": 66.6 }

// 		while (obj) {
// 			if (obj->type == cJSON_Object) {

// 				cJSON *objValue = obj->child;	// 获得 "game": "马里奥"
// 				while (objValue) {

// 					// 再通过类型去区分
// 					if (objValue->type == cJSON_String) {
// 						char *v_str = objValue->valuestring;
// 						printf("%s = %s\n", objValue->string, v_str);

// 					} else if (objValue->type == cJSON_Number) {
// 						double v_double = objValue->valuedouble;
// 						printf("%s = %.2f\n", objValue->string, v_double);
// 					}
// 					// 获取下一个元素
// 					objValue = objValue->next;
// 				}
// 			}
// 			// 获取下一组元素
// 			obj = obj->next;
// 		}
// 	}
// }


    /*解析： "eduaction":	[["小学", "初中"], ["高中", "大学"]],*/
    item = cJSON_GetObjectItem(root, "education");
    if (item != NULL){
        cJSON* val = NULL;
        cJSON* arr = NULL;
        int size = cJSON_GetArraySize(item);
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
    
//     {
// 	/*************** 方式一 ***************/
// 	item = cJSON_GetObjectItem(root, "eduaction");
// 	if (item != NULL) {
// 		int size = cJSON_GetArraySize(item);	// 获取的数组大小

// 		for (int i = 0; i < size; i++) {
// 			cJSON *arr = cJSON_GetArrayItem(item, i);		// 解析获得	[ "小学", "初中" ]

// 			if (arr != NULL && arr->type == cJSON_Array) {
// 				int arrSize = cJSON_GetArraySize(arr);

// 				for (int j = 0; j < arrSize; j++) {
// 					cJSON *arr2 = cJSON_GetArrayItem(arr, j);	// 再进一步解析就可以得到数组里面的元素了

// 					if (arr2 != NULL && arr2->type == cJSON_String) {
// 						v_str = arr2->valuestring;
// 						printf("education = %s\n", v_str);
// 					}
// 				}
// 			}
// 		}
// 	}

// 	/*************** 方式二 ***************/
// 	item = cJSON_GetObjectItem(root, "education");
// 	if (item != NULL) {
// 		cJSON *arr = item->child;	// 获得 [ "小学", "初中" ]

// 		while (arr) {
// 			if (arr->type == cJSON_Array) {
			
// 				cJSON *arrValue = arr->child;	// 获得 "小学"
// 				while (arrValue) {
// 					if (arrValue->type == cJSON_String) {
// 						char *v_str = arrValue->valuestring;
// 						printf("education = %s\n", v_str);
// 					}
// 					arrValue = arrValue->next;	// 获取下一个元素
// 				}
// 			}
// 			// 获取下一组
// 			arr = arr->next;
// 		}
// 	}
// }

    /*解析： "lagngues":	{
		"serialOne":	{
			"language":	"汉语",
			"grade":	10
		},
		"serialTwo":	{
			"language":	"英语",
			"grade":	6
		}
	},*/


    item = cJSON_GetObjectItem(root, "lagngues");
    char* arrStr[] = { "serialOne", "serialTwo"};
    if(item != NULL) {
        cJSON* val = NULL;
        cJSON* arr = NULL;
        int size = sizeof(arrStr) / sizeof(char*);
        for(int i = 0; i< size; i++) {
            val = cJSON_GetObjectItem(item, arrStr[i]);
            if ((val != NULL) && (val->type == cJSON_Object)){
                arr = cJSON_GetObjectItem(val, "language");
                if((arr != NULL) && (arr->type == cJSON_String)){
                    v_str = cJSON_Print(arr);
                        printf("education = %s\n", v_str);
                        free(v_str);
                        v_str = NULL;
                }

                arr = cJSON_GetObjectItem(val, "grade");
                if((arr != NULL) && (arr->type == cJSON_Number)){
                    v_int = arr->valueint;
                    printf("grade = %d\n", v_int);
                }
            }
        }
    }
        
            }


            
	// /*************** 方式一 ***************/
	// char *arrStr[] = { "serialOne", "serialTwo" };		// 可以提前定义需要解析的对象键，这样就可以使用for循环进行解析了

	// item = cJSON_GetObjectItem(root, "languages");
	// if (item != NULL) {
	// 	cJSON *val = NULL;
	// 	int size = sizeof(arrStr) / sizeof(char *);

	// 	// 通过遍历指针数组，获得每个对象的键，在进行解析操作（如果不使用for循环解析，那就老老实实的写代码将全部个数解析完毕）
	// 	for (int i = 0; i < size; i++) {
	// 		cJSON *obj1 = cJSON_GetObjectItem(item, arrStr[i]);

	// 		if (obj1 != NULL && obj1->type == cJSON_Object) {

	// 			val = cJSON_GetObjectItem(obj1, "language");
	// 			if (val != NULL && val->type == cJSON_String) {
	// 				v_str = val->valuestring;
	// 				printf("education = %s\n", v_str);
	// 			}

	// 			val = cJSON_GetObjectItem(obj1, "grade");
	// 			if (val != NULL && val->type == cJSON_Number) {
	// 				v_int = val->valueint;
	// 				printf("grade = %d\n", v_int);
	// 			}
	// 		}
	// 	}
	// }



	// /*************** 方式二 ***************/
	// // 在不知道键是什么的情况下 和 不知道有多少元素的情况下可用
	// item = cJSON_GetObjectItem(root, "languages");
	// if (item != NULL) {
	// 	// 获取到languages里的第一个子元素
	// 	cJSON *obj = item->child;	// 也就是："serialOne": { "language": "汉语", "grade": 10 }

	// 	while (obj) {
	// 		if (obj->type == cJSON_Object) {

	// 			// 获取到子元素的子元素
	// 			cJSON *value = obj->child;	// 也就是：{ "language": "汉语", "grade": 10 }

	// 			while (value) {
	// 				if (value->type == cJSON_String) {
	// 					printf("%s = %s\n", value->string, value->valuestring);

	// 				} else if (value->type == cJSON_Number) {
	// 					printf("%s = %d\n", value->string, value->valueint);
	// 				}
	// 				// 通过next可以自由获取里面的元素了
	// 				value = value->next;
	// 			}
	// 		}

	// 		// 获得下一组子元素
	// 		obj = obj->next;
	// 	}
	// }


    return 0;
}