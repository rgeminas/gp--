#include <stddef.h>
#ifndef __DARRAY_H
#define __DARRAY_H

#define darray_get(arr, i) ((arr)->base[(i)]) 

#define DARRAY_TYPEDECL(name, type) \
typedef struct darray_##name \
{ \
    type** base; \
    size_t allocated_mem; \
    size_t length; \
} darray_##name; 

#define DARRAY_IMPL(name, type) \
static darray_##name* \
darray_init_##name() \
{ \
    darray_##name* arr = (darray_##name*) malloc(sizeof(darray_##name)); \
    arr->base = (type**) malloc(sizeof(type*)); \
    arr->length = 0; \
    arr->allocated_mem = 1; \
    return arr; \
} \
 \
static void  \
darray_memcheck_##name(darray_##name* arr) \
{ \
    if (arr->length == arr->allocated_mem) \
    { \
        arr->allocated_mem <<= 1; \
        arr->base = (type**) realloc(arr->base, arr->allocated_mem * sizeof(type*)); \
    } \
    if (arr->length < (arr->allocated_mem >> 1)) \
    { \
        arr->allocated_mem >>= 1; \
        arr->base = (type**) realloc(arr->base, arr->allocated_mem * sizeof(type*)); \
    } \
} \
 \
static void  \
darray_push_back_##name(darray_##name* arr,  \
           type* s) \
{ \
    arr->base[arr->length++] = s; \
    darray_memcheck_##name(arr); \
} \
 \
/* TODO: This is very fucking inefficient. Turn this into a deque */ \
/* after it's presented to the professor */\
static void  \
darray_push_front_##name(darray_##name* arr,  \
                  type* s) \
{ \
    type* temp = darray_get(arr, 0); \
    for (size_t i = 0; i < arr->length; i++) \
    { \
        type* temp2 = darray_get(arr, i+1); \
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
            arr->base[k] = arr->base[j]; \
        } \
    } \
    arr->length--; \
    darray_memcheck_##name(arr); \
}

#endif // __DARRAY_H
