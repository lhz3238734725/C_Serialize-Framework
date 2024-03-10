#include <stdio.h>
#include "sf-pack.h"

static int __sf_pack(void *p IN, uint8_t size IN, uint8_t **pbuf OUT, int *plen OUT){

    uint8_t data[8] = { 0 };
    int len = 0;
    switch (size)
    {
    case 1:
        *(uint8_t*)data = *(uint8_t*)p;
        len = 1;
        printf("[pack] len: %d bytes, data: %d, address: %p\n", len, *(uint8_t*)data, data);
        break;
    case 2:
        *(uint16_t*)data = *(uint16_t*)p;
        len = 2;
        printf("[pack] len: %d bytes, data: %d, address: %p\n", len, *(uint16_t*)data, data);
        break;
    case 4:
        *(uint32_t*)data = *(uint32_t*)p;
        len = 4;
        printf("[pack] len: %d bytes, data: %d, address: %p\n", len, *(uint32_t*)data, data);
        break;
    case 8:
        *(uint64_t*)data = *(uint64_t*)p;
        len = 8;
        printf("[pack] len: %d bytes data, data: %ld, address: %p\n", len, *(uint64_t*)data, data);
        break;
    default:
        printf("size '%d' is not supported\n", size);
        return SF_PARAM_ERROR;
    }
    void *b = realloc(*pbuf, *plen + len);
    if(!b) return SF_MEMORY_ERROR;

    *pbuf = (uint8_t*)b;
    memcpy(*pbuf + *plen, data, len);
    *plen += len;

    return SF_SUCCESS;
}

static int __sf_unpack(void *p, int size, const uint8_t *buf, int len){

    switch (size)
    {
    case 1:
        if(len >= 1){
            *(uint8_t*)p = *(uint8_t*)buf;
            printf("[unpack] len: 1 bytes, data: %d\n", *(uint8_t*)buf);
            return 1;
        }
        break;
    case 2:
        if(len >= 2){
            *(uint16_t*)p = *(uint16_t*)buf;
            printf("[unpack] len: 2 bytes, data: %d\n", *(uint16_t*)buf);
            return 2;
        }
        break;
    case 4:
        if(len >= 4){
            *(uint32_t*)p = *(uint32_t*)buf;
            printf("[unpack] len: 4 bytes, data: %d\n", *(uint32_t*)buf);
            return 4;
        }
        break;
    case 8:
        if(len >= 8){
            *(uint64_t*)p = *(uint64_t*)buf;
            printf("[unpack] len: 8 bytes, data: %ld\n", *(uint64_t*)buf);
            return 8;
        }
        break;
    default:
        printf("size '%d' is not supported\n", size);
        return SF_PARAM_ERROR;
    }
}

int sf_pack(void *p IN, sf_pack_info_t *infos IN, uint64_t n_infos IN, uint8_t **pbuf OUT, int *plen OUT){
    
    if(!p || !infos || n_infos < 1 || !pbuf || !plen){
        printf("[pack]: param error\n");
        return SF_PARAM_ERROR;
    }

    if(!*pbuf){
        *plen = 0;
    }

    int ret = SF_SUCCESS;

    for (uint64_t i = 0; i < n_infos; i++)
    {
        printf("[pack_info] name: '%s'  size:%ld offset:%ld noffset:%ld msize:%ld n_members:%ld members:%p\n", infos[i].name, infos[i].size, \
                infos[i].offset, infos[i].noffset, infos[i].msize, infos[i].n_members, infos[i].members);
        
        // 处理数组
        if(infos[i].noffset >= 0){
            void *arry = *(void**)(p + infos[i].offset);
            uint64_t n = *(uint64_t*)(p + infos[i].noffset);
            printf("arry: %p number: %ld\n", arry, n);
            if(n <= 0) continue;
            
            if(!infos[i].n_members){
                // 常规数据数组
                for (uint64_t j = 0; j < n; j++)
                {
                    ret = __sf_pack(arry + infos[i].msize * j, infos[i].msize, pbuf, plen);
                    if(ret != SF_SUCCESS)
                        return ret;
                }
                
            }else{
                // 结构体数据数组
                for (uint64_t j = 0; j < n; j++)
                {
                    ret = sf_pack(*(void**)(arry + infos[i].msize * j), infos[i].members, infos[i].n_members, pbuf, plen);
                    if(ret != SF_SUCCESS)
                        return ret;
                }
            }

            continue;
        }

        // 处理结构体数据
        if(infos[i].n_members){
            sf_pack(*(void **)(p + infos[i].offset), infos[i].members, infos[i].n_members, pbuf, plen);
            continue;
        }

        // 处理常规数据
        ret = __sf_pack(p + infos[i].offset, infos[i].size, pbuf, plen);
        if(ret != SF_SUCCESS)
            return ret;

    }
    return ret;
}

int sf_unpack(void **pp OUT, sf_pack_info_t *infos IN, uint64_t n_infos IN, const uint8_t *buf IN, int len IN){

    if(!pp || !infos || !buf || n_infos < 1 || len < 1){
        printf("[unpack]: param error\n");
        return SF_PARAM_ERROR;
    }

    int ret = SF_SUCCESS;
    int size = infos[n_infos - 1].offset + infos[n_infos - 1].size;
    void *p = calloc(1, size);
    uint64_t k = 0;
    if(!p)
        return SF_MEMORY_ERROR;
    printf("[unpcak] alloc memory %p\n", p);

    for(uint64_t i = 0; i < n_infos; i++){
        printf("[unpack_info] name: '%s' size:%ld offset:%ld noffset:%ld msize:%ld n_members:%ld members:%p\n", infos[i].name, infos[i].size, \
                infos[i].offset, infos[i].noffset, infos[i].msize, infos[i].n_members, infos[i].members);
        // 处理数组
        if(infos[i].noffset >= 0){

            uint64_t n = *(uint64_t*)(p + infos[i].noffset);
            if (n <= 0 ) continue;
            
            void *sub_arry = calloc(n ,infos[i].msize);
            if(!sub_arry)
                return SF_MEMORY_ERROR;
            
            printf("[unpcak] alloc memory %p\n", sub_arry);
            *(void**)(p + infos[i].offset) = sub_arry;

            if(infos[i].n_members){
                // 结构体数组
                for (uint64_t j = 0; j < n; j++)
                {
                    ret = sf_unpack(sub_arry + infos[i].msize * j, infos[i].members, infos[i].n_members, buf + k, len - k);
                    if(ret < 0)
                        return ret; 
                    k += ret;
                }
            }else{
                // 普通数组
                for (uint64_t j = 0; j < n; j++)
                {
                    ret = __sf_unpack(sub_arry + infos[i].msize * j, infos[i].msize, buf + k, len -k);
                    if(ret < 0)
                        return ret;
                    k += ret;
                }
                
            }

            continue;
        }

        // 处理结构体数数据
        if(infos[i].n_members){

            ret = sf_unpack((void**)(p + infos[i].offset), infos[i].members, infos[i].n_members, buf + k, len - k);
            if(ret < 0)
                return ret;
            k += ret;
            continue;
        }

        // 处理普通数据
        ret = __sf_unpack(p + infos[i].offset, infos[i].size, buf + k, len -k);
        if(ret < 0)
            return ret; 
        k += ret;

    }

    *pp = p;

    return k;
}

int sf_unpack_free(void *p IN, sf_pack_info_t *infos IN, uint64_t n_infos IN){

    if(!p || !infos || n_infos <= 0){
        printf("[unpack_free]: param error\n");
        return SF_PARAM_ERROR;
    }

    int ret = SF_SUCCESS;
    for (uint64_t i = 0; i < n_infos; i++)
    {
        printf("[unpack_free_info] name: '%s' size:%ld offset:%ld noffset:%ld msize:%ld n_members:%ld members:%p\n", infos[i].name, infos[i].size, \
                infos[i].offset, infos[i].noffset, infos[i].msize, infos[i].n_members, infos[i].members);

        // 数组数据
        if(infos[i].noffset >= 0){
            void* array = *(void**)(p + infos[i].offset);
            uint64_t n = *(uint64_t*)(p + infos[i].noffset);

            if(infos[i].n_members){
                //结构体数组  
                for (uint64_t j = 0; j < n; j++)
                {             
                    ret = sf_unpack_free(*(void**)(array + infos[i].msize * j), infos[i].members, infos[i].n_members); 
                    if(ret != SF_SUCCESS)
                        return ret; 
                }
            }

            printf("[unpack_free] free memory %p\n", array);
            free(array);
            array = NULL;
            continue;
        }


        // 结构体数据
        if(infos[i].n_members){
            ret = sf_unpack_free(*(void**)(p + infos[i].offset), infos[i].members, infos[i].n_members); 
            if(ret != SF_SUCCESS)
                return ret;
        }
    }
    
    // 释放内存
    printf("[unpack_free] free memory %p\n", p);
    free(p);
    p = NULL;
    return SF_SUCCESS;
} 