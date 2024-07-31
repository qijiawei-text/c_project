#include  "cJSON.h"
#include <stdio.h>
#include <stdlib.h>


// 第一组数据
/*  {
        "name":	"漆嘉伟",
        "age":	22,
        "weight":	65.3,
        "address":	{
            "country":	"China",
            "home":	"武汉",
            "zip-code":	11111
        },
        "skill":	["C", "Java", "Python", 8086],
        "student":	true
    }




    int main(void)
    {
        // 创建对象
        cJSON* cjson_example = NULL;
        cJSON* cjson_address = NULL;
        cJSON* cjson_skill = NULL;

        //first head_node
        cjson_example = cJSON_CreateObject();

        //添加键值对
        cJSON_AddStringToObject(cjson_example, "name", "漆嘉伟");
        cJSON_AddNumberToObject(cjson_example, "age", 22);
        cJSON_AddNumberToObject(cjson_example, "weight", 65.3);

        //另外一个对象
        cjson_address = cJSON_CreateObject();
        cJSON_AddStringToObject(cjson_address, "country", "China");
        cJSON_AddStringToObject(cjson_address, "home", "武汉");
        cJSON_AddNumberToObject(cjson_address, "zip-code", 11111);

        //连接到主对象
        cJSON_AddItemToObject(cjson_example, "address", cjson_address);

        //创建数组
        cjson_skill = cJSON_CreateArray();
        cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "C" ));
        cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "Java" ));
        cJSON_AddItemToArray(cjson_skill, cJSON_CreateString( "Python" ));
        cJSON_AddItemToArray(cjson_skill,cJSON_CreateNumber(8086));
        cJSON_AddItemToObject(cjson_example, "skill", cjson_skill);

        cJSON_AddBoolToObject(cjson_example, "student", cJSON_True);

        //打印json数据
        char* str = cJSON_Print(cjson_example);
        printf("%s\n", str);

        return 0;
    }   */


// 第二组数据

int main(void)
{
    cJSON* cjson_example = NULL;
    cjson_example = cJSON_CreateObject();

    cJSON_AddStringToObject(cjson_example, "name", "漆嘉伟");
    cJSON_AddNumberToObject(cjson_example, "age", 22);

    cJSON* cjson_interest = NULL;
    cjson_interest = cJSON_CreateObject();

    //创建sport,并将sport插入到interest。
    cJSON* cjson_sport = NULL;
    cjson_sport = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_sport, cJSON_CreateString("篮球"));
    cJSON_AddItemToArray(cjson_sport, cJSON_CreateString("足球"));
    cJSON_AddItemToObject(cjson_interest, "sport", cjson_sport);

    //创建book，并将book插入到interest。
    cJSON* cjson_book = NULL;
    cjson_book = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_book, cJSON_CreateString("unix高级环境编程"));
    cJSON_AddItemToArray(cjson_book, cJSON_CreateString("c prime plus"));
    cJSON_AddItemToObject(cjson_interest, "book", cjson_book);

    //将interest插入到根example
    cJSON_AddItemToObject(cjson_example, "interest", cjson_interest);

    //创建color，并插入example
    cJSON* cjson_color = NULL;
    cjson_color = cJSON_CreateArray();
    cJSON_AddItemToArray(cjson_color, cJSON_CreateString("black"));
    cJSON_AddItemToArray(cjson_color, cJSON_CreateString("white"));
    cJSON_AddItemToObject(cjson_example, "color", cjson_color);

    //创建like
    cJSON* cjson_like = NULL;
    cjson_like = cJSON_CreateArray();

    //创建game
    cJSON* cjson_game_1 = NULL;
    cjson_game_1 = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_game_1, "game", "塞尔达旷野之息");
    cJSON_AddNumberToObject(cjson_game_1, "price", 256.8);

    cJSON* cjson_game_2 = NULL;
    cjson_game_2 = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_game_2, "game", "Eding Ring");
    cJSON_AddNumberToObject(cjson_game_2, "price", 299);

    //将game插入like,并把like插入example
    cJSON_AddItemToArray(cjson_like, cjson_game_1);
    cJSON_AddItemToArray(cjson_like, cjson_game_2);
    cJSON_AddItemToObject(cjson_example, "like", cjson_like);

    //创建education,并加入example
    cJSON* cjson_education = NULL;
    cjson_education = cJSON_CreateArray();

    cJSON* education_1 = NULL;
    education_1 = cJSON_CreateArray();
    cJSON_AddItemToArray(education_1, cJSON_CreateString("小学"));
    cJSON_AddItemToArray(education_1, cJSON_CreateString("初中"));

    cJSON* education_2 = NULL;
    education_2 = cJSON_CreateArray();
    cJSON_AddItemToArray(education_2, cJSON_CreateString("高中"));
    cJSON_AddItemToArray(education_2, cJSON_CreateString("大学"));

    cJSON_AddItemToArray(cjson_education, education_1);
    cJSON_AddItemToArray(cjson_education, education_2);

    cJSON_AddItemToObject(cjson_example, "education", cjson_education);

    //创建languages
    cJSON* cjson_languages = NULL;
    cjson_languages = cJSON_CreateObject();

    //创建serialOne，serialTwo
    cJSON* serialOne = NULL;
    serialOne = cJSON_CreateObject();
    cJSON_AddStringToObject(serialOne, "language", "汉语");
    cJSON_AddNumberToObject(serialOne, "grade", 10);

    cJSON* serialTwo = NULL;
    serialTwo = cJSON_CreateObject();
    cJSON_AddStringToObject(serialTwo, "language", "英语");
    cJSON_AddNumberToObject(serialTwo, "grade", 6);

    //将serialone、serialtwo加入到languages
    cJSON_AddItemToObject(cjson_languages, "serialOne", serialOne);
    cJSON_AddItemToObject(cjson_languages, "serialTwo", serialTwo);
    cJSON_AddItemToObject(cjson_example, "lagngues", cjson_languages);

    cJSON_AddItemToObject(cjson_example, "vip", cJSON_CreateTrue());
    cJSON_AddItemToObject(cjson_example, "address", cJSON_CreateNull());


    char* str = cJSON_Print(cjson_example);
    printf("%s\n", str);
    free(str);
    cJSON_Delete(cjson_example);
    return 0;
}