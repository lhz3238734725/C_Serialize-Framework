#ifndef SF_PACK_H
#define SF_PACK_H

#include "sf-def.h"
/*
* 实现思路：将结构体中的成员信息保存下来，通过这个成员信息实现序列化和反序列化
* 支持标准数据类型 自定义结构体 数组
*/

typedef struct sf_pack_info_s sf_pack_info_t;

// 结构体成员信息 
struct sf_pack_info_s
{
    char *name;                 // 名称
    uint64_t size;              // 偏移量
    uint64_t offset;            // 大小
    int64_t noffset;            // 用于数组，数组长度的偏移量
    int64_t msize;              // 数组成员的大小
    uint64_t n_members;         // 用于自定义结构体，自定义结构体成员信息数组长度
    sf_pack_info_t *members;    // 用于自定义结构体，自定义结构体成员信息数组
};

int sf_pack(void *p IN, sf_pack_info_t *infos IN, uint64_t n_infos IN, uint8_t **pbuf OUT, int *plen OUT);
int sf_unpack(void **pp OUT, sf_pack_info_t *infos IN, uint64_t n_infos IN, const uint8_t *buf IN, int len IN);
int sf_unpack_free(void *p IN, sf_pack_info_t *infos IN, uint64_t n_infos IN); 

// 结构体某个成员的大小
#define SF_SIZEOF(type, member)         sizeof(((type*)0)->member)

// 定义标准数据类型 标准数据类型数组
#define SF_PACK_DEF_VAR(type, var)      type var
#define SF_PACK_DEF_VARS(type, vars)    uint64_t n_##vars; type* vars

// 定义自定义结构体类型 自定义结构体类型数组
#define SF_PACK_DEF_OBJ(type, obj)      type* obj
#define SF_PACK_DEF_OBJS(type, objs)    uint64_t n_##objs; type** objs

// 获取结构体成员数组的长度
#define SF_PACK_N_INFOS(type)           sizeof(sf_pack_info_##type)/sizeof(sf_pack_info_##type[0])

// 定义结构体成员信息数组
#define SF_PACK_INFO_TYPE(type) \
static sf_pack_info_t sf_pack_info_##type[] = { 

// 定义标准数据类型成员信息
#define SF_PACK_INFO_DEF_VAR(type, var) \
{ #var,  SF_SIZEOF(type, var), offsetof(type, var), -1, -1, 0, NULL } 

// 定义标准数据类型数组成员信息
#define SF_PACK_INFO_DEF_VARS(type, vars, vtype) \
{ "n_"#vars, SF_SIZEOF(type, n_##vars), offsetof(type, n_##vars), -1, -1, 0, NULL }, \
{ #vars, SF_SIZEOF(type, vars), offsetof(type, vars), offsetof(type, n_##vars), sizeof(vtype), 0, NULL }

// 定义自定义结构体成员信息
#define SF_PACK_INFO_DEF_OBJ(type, obj, objtype) \
{ #obj, SF_SIZEOF(type, obj), offsetof(type, obj), -1, -1, SF_PACK_N_INFOS(objtype), sf_pack_info_##objtype }

// 定义自定义结构体数组成员信息
#define SF_PACK_INFO_DEF_OBJS(type, objs, objtype) \
{ "n_"#objs, SF_SIZEOF(type, n_##objs), offsetof(type, n_##objs), -1, -1, 0, NULL }, \
{ #objs, SF_SIZEOF(type, objs), offsetof(type, objs), offsetof(type, n_##objs), sizeof(objtype*), SF_PACK_N_INFOS(objtype), sf_pack_info_##objtype }

// 结束定义
#define SF_PACK_INFO_END(type) \
}; \
static int sf_##type##_pack(type *p, uint8_t **pbuf, int *plen) \
{ \
    sf_pack(p, sf_pack_info_##type, SF_PACK_N_INFOS(type), pbuf, plen); \
} \
static int sf_##type##_unpack(type **pp, uint8_t *buf, int len) \
{ \
    sf_unpack((void**)pp, sf_pack_info_##type, SF_PACK_N_INFOS(type), buf, len); \
} \
static void sf_##type##_unpack_free(type *p) \
{ \
    sf_unpack_free(p, sf_pack_info_##type, SF_PACK_N_INFOS(type)); \
}

#endif