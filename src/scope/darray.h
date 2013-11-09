/*
 Generic dynamic array function and structure generator.

 Usage:

DARRAY_DEFINE(int32, int)

int main(void)
{
   darray(int32)* arr = darray_init(int32)();

   darray_push_back(int32)(arr, 5);
   darray_push_back(int32)(arr, 6);

   darray_push_front(int32)(arr, 4);
   darray_push_front(int32)(arr, 3);

   darray_remove(int32)(arr, 1);

   printf("%d", darray_get_at(int32)(arr, 2)); // 5

   darray_get_at(int32)(arr, 2) = 8;

   printf("%d", darray_get_at(int32)(arr, 2)); // 8 

   darray_free(int32)(arr);
   return 0;
}
*/
#ifndef __DARRAY_H
#define __DARRAY_H
#include <stddef.h>

#define darray_get_at(id, arr, i) darray_get(arr, i)
#define darray_get(arr, i) ((arr)->base[(i)]) 

// Function aliases to look like C++ templates.
#define darray(id) darray_##id
#define darray_init(id) darray_init_##id
#define darray_push_back(id) darray_push_back_##id
#define darray_push_front(id) darray_push_front_##id
#define darray_remove(id) darray_remove_##id
#define darray_free(id) darray_remove_##id

// This is ithe macro you should use to define the generic type.
// * 'name' is how you refer to the darray type, for example,
//     you can't set name to be int*, because that doesn't fit
//     into a function identifier.
// * 'type' is the proper type in the container.
//
#define DARRAY_DEFINE(name, type) \
    DARRAY_TYPEDECL(name, type) \
    DARRAY_IMPL(name, type)

#define DARRAY_TYPEDECL(name, type) \
typedef struct darray_##name \
{ \
    type* base; \
    size_t allocated_mem; \
    size_t length; \
} darray_##name; 

#define DARRAY_IMPL(name, type) \
static darray_##name* \
darray_init_##name() \
{ \
    darray_##name* arr = (darray_##name*) malloc(sizeof(darray_##name)); \
    arr->base = (type*) malloc(sizeof(type)); \
    arr->length = 0; \
    arr->allocated_mem = 1; \
    return arr; \
} \
 \
static void \
darray_free_##name(darray_##name* arr) \
{\
    free(arr->base); \
    free(arr); \
} \
 \
static void  \
darray_memcheck_##name(darray_##name* arr) \
{ \
    if (arr->length == arr->allocated_mem) \
    { \
        arr->allocated_mem <<= 1; \
        arr->base = (type*) realloc(arr->base, arr->allocated_mem * sizeof(type)); \
    } \
    if (arr->length < (arr->allocated_mem >> 1)) \
    { \
        arr->allocated_mem >>= 1; \
        arr->base = (type*) realloc(arr->base, arr->allocated_mem * sizeof(type)); \
    } \
} \
 \
static void  \
darray_push_back_##name(darray_##name* arr,  \
           type s) \
{ \
    arr->base[arr->length++] = s; \
    darray_memcheck_##name(arr); \
} \
 \
/* TODO: This is very fucking inefficient. Turn this into a deque */ \
/* after it's presented to the professor */\
static void  \
darray_push_front_##name(darray_##name* arr,  \
                  type s) \
{ \
    type temp = darray_get(arr, 0); \
    for (size_t i = 0; i < arr->length; i++) \
    { \
        type temp2 = darray_get(arr, i+1); \
        darray_get(arr, i+1) = temp; \
        temp = temp2; \
    } \
    darray_get(arr, 0) = s; \
    arr->length++; \
    darray_memcheck_##name(arr); \
} \
 \
static void  \
darray_remove_##name(darray_##name* arr,  \
              size_t i) \
{ \
    for (size_t j = 0, k = 0; j < arr->length; j++) \
    { \
        if (j == i) continue; \
        else \
        { \
            arr->base[k++] = arr->base[j]; \
        } \
    } \
    arr->length--; \
    darray_memcheck_##name(arr); \
}

#endif // __DARRAY_H
