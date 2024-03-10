# C_Serialize-Framework
C语言实现一个序列化框架

## 项目目录结构
```
bin  --> 存放编译生成的可执行文件
lib  --> 存放编译生成的库文件
src  --> 项目代码目录
test --> 测试代码目录
```
## 项目编译
```
mkdir build && cd build
cmake ..
make
```
## 接口说明

### 定义结构体数据
```
/*
* 定义一个标准类型的变量
* type: 变量类型
* var:  变量名
*/
SF_PACK_DEF_VAR(type, var)

// 示例 
// SF_PACK_DEF_VAR(int, a); ==> int a;


/*
* 定义一个标准类型数组
* type: 变量类型
* vars:  变量名
*/
SF_PACK_DEF_VARS(type, vars)

// 示例 
// SF_PACK_DEF_VARS(int, vals); ==> uint64_t n_vals; int *vals;


/*
* 定义一个自定义结构体变量
* type: 变量类型
* obj:  变量名
*/
SF_PACK_DEF_OBJ(type, obj)

// 示例
// A 是一个结构体类型 
// SF_PACK_DEF_OBJ(A, a); ==> A *a;

/*
* 定义一个自定义结构体变量数组
* type: 变量类型
* objs:  变量名
*/
SF_PACK_DEF_OBJS(type, objs)

// 示例
// A 是一个结构体类型 
// SF_PACK_DEF_OBJS(A, as); ==> uint64_t n_as; A **as;

```

### 定义结构体数据信息

```
/*
* 定义结构体成员信息数组
* type: 结构体类型
*/
SF_PACK_INFO_TYPE(type)

/*
* 定义标准数据类型成员信息
* type: 结构体类型
* var:  变量名
*/
SF_PACK_INFO_DEF_VAR(type, var)

/*
* 定义标准数据类型数组成员信息
* type:  结构体类型
* vars:  变量名
* vtype: 数组成员子类型
*/
SF_PACK_INFO_DEF_VARS(type, vars, vtype)

/*
* 定义自定义结构体成员信息
* type:  结构体类型
* obj:  变量名
* vtype: 结构体子类型
*/
SF_PACK_INFO_DEF_OBJ(type, obj, objtype)

/*
* 定义自定义结构体数组成员信息
* type:  结构体类型
* objs:  变量名
* vtype: 结构体子类型
*/
SF_PACK_INFO_DEF_OBJS(type, objs, objtype)

// 结束定义
#define SF_PACK_INFO_END(type) 
```

### 接口说明
```
// type 可以改变，指要转换的结构体类型

/*
* 将结构体类型转成二进制数据
* p:    结构体数据地址             输入参数
* pbuf: 二进制数据缓冲区的地址      输出参数
* plen: 二进制数据缓冲区的长度      输出参数
*/
int sf_type_pack(void *p IN, uint8_t **pbuf OUT, int *plen OUT);

/*
* 将二进制数据转成结构体数据
* p:    结构体数据地址             输出参数
* pbuf: 二进制数据缓冲区的地址      输入参数
* plen: 二进制数据缓冲区的长度      输入参数
*/
int sf_type_unpack(void **pp OUT, const uint8_t *buf IN, int len IN);

/*
* 释放解包生成的结构体
* p:    二进制数据缓冲区的地址       输入参数
*/
int sf_type_unpack_free(void *p IN); 
```

## 使用示例
``` c
#include "sf-pack.h"
#include <stdio.h>
typedef struct A
{
    SF_PACK_DEF_VAR(int, a1);
    SF_PACK_DEF_VAR(long, a2);
    SF_PACK_DEF_VAR(char, a3);
} A;

SF_PACK_INFO_TYPE(A)
SF_PACK_INFO_DEF_VAR(A, a1),
SF_PACK_INFO_DEF_VAR(A, a2),
SF_PACK_INFO_DEF_VAR(A, a3),
SF_PACK_INFO_END(A)

typedef struct B
{
    SF_PACK_DEF_VAR(int, x);
    SF_PACK_DEF_VAR(int, y);
    SF_PACK_DEF_VAR(int, z);
    SF_PACK_DEF_OBJ(A, a);
    SF_PACK_DEF_OBJS(A, as);
} B;

SF_PACK_INFO_TYPE(B)
SF_PACK_INFO_DEF_VAR(B, x),
SF_PACK_INFO_DEF_VAR(B, y),
SF_PACK_INFO_DEF_VAR(B, z),
SF_PACK_INFO_DEF_OBJ(B, a, A),
SF_PACK_INFO_DEF_OBJS(B, as, A),
SF_PACK_INFO_END(B)

typedef struct TEST
{
    SF_PACK_DEF_VAR(uint8_t, val_1);
    SF_PACK_DEF_VAR(uint16_t, val_2);
    SF_PACK_DEF_VAR(uint32_t, val_3);
    SF_PACK_DEF_VAR(uint64_t, val_4);
    SF_PACK_DEF_VARS(int, val_ints);
    SF_PACK_DEF_OBJ(B, b);
    SF_PACK_DEF_OBJS(B, bs);
} TEST;

SF_PACK_INFO_TYPE(TEST)
SF_PACK_INFO_DEF_VAR(TEST, val_1),
SF_PACK_INFO_DEF_VAR(TEST, val_2),
SF_PACK_INFO_DEF_VAR(TEST, val_3),
SF_PACK_INFO_DEF_VAR(TEST, val_4),
SF_PACK_INFO_DEF_VARS(TEST, val_ints, int),
SF_PACK_INFO_DEF_OBJ(TEST, b, B),
SF_PACK_INFO_DEF_OBJS(TEST, bs, B),
SF_PACK_INFO_END(TEST)

int main(int argc, char **argv){

    TEST test;
    test.val_1 = 1;
    test.val_2 = 2;
    test.val_3 = 3;
    test.val_4 = 4;

    int ints[] = {1, 2, 3};
    test.n_val_ints = 0;
    test.val_ints = NULL;

    A a = {100, 200, 'a'};
    A a1 = {300, 400, 'b'};
    A a2 = {500, 600, 'c'};
    A a3 = {700, 800, 'd'};
    A *as[] = {
        &a1, &a2, &a3,
    };

    B b = { 7, 8 ,9 };
    b.a = &a;
    b.n_as = 0;
    b.as = NULL;
    B b1 = { 10, 10 ,10, &a, 3, as };
    B b2 = { 20, 20 ,20, &a, 3, as };
    B b3 = { 30, 30 ,30, &a, 3, as };
    test.b = &b;

    B *bs[] ={
        &b1, &b2, &b3,
    };
    test.n_bs = 3;
    test.bs = bs;

    uint8_t *buf = NULL;
    int len = 0;
    sf_TEST_pack(&test, &buf, &len);
    printf("get %d bytes buf\n", len);

    TEST *test1 = NULL;
    sf_TEST_unpack(&test1, buf, len);
    
    sf_TEST_unpack_free(test1);

    return 0;
}
```

