#include <stdio.h>
#include <unistd.h>
#include <math.h>
#define gold  100


static int people[104] = {0};


void my_print(int num){
    for(int i = num; i > 0; i--){
        printf("%d, ",people[i]); 
    }
    printf("\n");
}


/*当人数在 1 ~ 5的时候*/
void fn_1(int num){
    switch (num) {
        case 1:
            printf("the number of people %d: 100\n", num);
            break;
        case 2:
            printf("the number of people %d: -1\n", num);
            break;
        case 3:
            printf("the number of people %d: 100, 0, 0\n", num);
            break;
        case 4:
            printf("the number of people %d: 98, 0, 1, 1\n", num);
            break;
        case 5:
            printf("the number of people %d: 97, 0, 1, 2, 0\n", num);
            break;
    }
}


/*当人数在 6 ~ 101时*/
void fn_2(int num){
    int give_gold_people = (num - 3 + 1) / 2; 
    //printf("%d, ", give_gold_people);
    people[num] = gold - (give_gold_people *2 + 1);
    //printf("%d, ", people[num]);
    people[num -1] = 0;
    people[num -2]= 1;
    int i;
	for(i = 1; i <= (num-3); i++)
	{
		if( i%2 == 1){
            people[i] = 2;
        }
	}
}

/*当人数在 102 ~ 103时*/
void fn_3(int num){
    int give_gold_people = ceil((num-3)/2); 
    people[num] = 0;
    people[num -1] = 1;
    people[num -2]= 1;
    int i;
	for(i = 1; (i <= (num-3))&&( i <= 98); i++)
	{
		if( i%2 == 1){
            people[i] = 2;
        }
	}
}

/*当人数大于 104 时*/
void fn_4(int num){
    int left = 0, right = 3;
    int flag = 0;
    int temp = num - 98;
    int j = 0;
    left = temp - right;
    while ( left > 0)
    {
        // printf("right : %d \n", right);
        // printf("left : %d \n", left);
        // printf("temp: %d \n", temp);
        if( (temp - right) == (right + 1) )
        {
            flag = 1;
            break;
        }
        left = temp -right;
        j = right - 1;
        right = right + j;
    }

    if( 1 == flag){
        fn_3(103);
        printf("the number of people is %d: ", num); 
        for(int i = num; i > 103; i--)
            printf("0, "); 
        my_print(num = 103);
    }else {
        printf("the number of people is %d: -1 \n", num);
    }
}

int main()
{
    int num = 0;
    printf("please input the number of people: ");
    while(scanf("%d",&num)!=EOF){
        if( (0 < num) && (num <=5)){
            fn_1(num);
        }
        if( (5 < num) && (num <= 101)){
            fn_2(num);
            printf("the number of people is %d: ", num);
            my_print(num);
        }
        if( (101 < num) && (num <= 103)){
            fn_3(num);
            printf("the number of people is %d: ", num);
            my_print(num);
        }
        if( 104 == num )
        {
            printf("the number of people is %d: -1\n", num);
        }
        if( num > 104)
        {
            printf("the number of people is %d: ", num);
            fn_4(num);
        }
        system("pause");
        printf("\nplease input the number of people: ");
        memset(people, 0, sizeof(people));
    }
    return 0;
}