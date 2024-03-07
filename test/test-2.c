#include <stdio.h>
#include <stdint.h>

typedef struct A
{
    int val_1;
    long val_2;
    double val_3;
} A;

typedef struct vinfo_t
{
    const char *name;
    uint64_t offset;
    uint64_t size;
} vinfo_t;

// 获取结构体元素的偏移量
#define OFFSET(type, member) ((long)&(((type*)0)->member))
// 获取结构体元素的大小
#define SIZEOF(type, member) sizeof(((type*)0)->member)
// 初始化一个vinfo_t结构体
#define VINFO(type, member) {#type, OFFSET(type, member), SIZEOF(type, member)}

vinfo_t A_info[] = {
    VINFO(A, val_1),
    VINFO(A, val_2),
    VINFO(A, val_3)
};

int main(int argc, char **argv){

    printf("val1 offset:%ld size:%ld\n" ,OFFSET(A, val_1), SIZEOF(A, val_1));
    printf("val2 offset:%ld size:%ld\n" ,OFFSET(A, val_2), SIZEOF(A, val_2));
    printf("val3 offset:%ld size:%ld\n" ,OFFSET(A, val_3), SIZEOF(A, val_3));

    for (int i = 0; i < sizeof(A_info) / sizeof(A_info[0]); i++)
    {
        printf("name: %s offset: %ld size: %ld\n", A_info[i].name, A_info[i].offset, A_info[i].size);
    }
    
    return 0;
}