#include "../src/sf-pack.h"
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