#include "scope/symrec.h"
#include "parser/base_parser.h"
#include <stdlib.h>
#include <stdio.h>

extern char** secondary_tokens;

symbol_stack* scope_stack;

// START DARRAY FUNCTIONS HERE

darray_symrec* 
darray_init()
{
    darray_symrec* arr = (darray_symrec*) malloc(sizeof(darray_symrec));
    arr->base = (symrec**) malloc(sizeof(symrec*));
    arr->length = 0;
    arr->allocated_mem = 1;
    return arr;
}

void 
darray_memcheck(darray_symrec* arr)
{
    if (arr->length == arr->allocated_mem)
    {
        arr->allocated_mem <<= 1;
        arr->base = (symrec**) realloc(arr->base, arr->allocated_mem * sizeof(symrec*));
    }
    if (arr->length < (arr->allocated_mem >> 1))
    {
        arr->allocated_mem >>= 1;
        arr->base = (symrec**) realloc(arr->base, arr->allocated_mem * sizeof(symrec*));
    }
}

void 
darray_push_back(darray_symrec* arr, 
           symrec* s)
{
    arr->base[arr->length++] = s;
    darray_memcheck(arr);
}

// TODO: This is very fucking inefficient. Turn this into a deque
// after it's presented to the professor
void 
darray_push_front(darray_symrec* arr, 
                  symrec* s)
{
    symrec* temp = darray_get(arr, 0);
    for (size_t i = 0; i < arr->length; i++)
    {
        symrec* temp2 = darray_get(arr, i+1);
        darray_get(arr, i+1) = temp;
        temp = temp2;
    }
    darray_get(arr, 0) = s;
    arr->length++;
    darray_memcheck(arr);
}

void 
darray_remove(darray_symrec* arr, 
              size_t i)
{
    for (size_t j = 0, k = 0; j < arr->length; j++)
    {
        if (j == i) continue;
        else
        {
            arr->base[k] = arr->base[j];
        }
    }
    arr->length--;
    darray_memcheck(arr);
}

symrec*
darray_find_id(darray_symrec* arr,
               int id)
{
    for (int i = 0; i < arr->length; i++)
    {
        if (darray_get(arr, i)->id == id)
        {
            return darray_get(arr, i);
        }
    }
    return NULL;
}
// END DARRAY FUNCTIONS HERE

void
print_symrec(symrec* s)
{
    if (s == NULL) return;
    fprintf(stderr, "sp: %d // id: %s // type: %d\n", s->spec, secondary_tokens[s->id], s->type);
    if (s->parameter_list != NULL)
    {
        fprintf(stderr, "PARAMS: \n");
        for (size_t i = 0; i < s->parameter_list->length; i++)
        {
            fprintf(stderr, "    ");
            print_symrec(darray_get(s->parameter_list, i));
        }
    }
}

void
print_table()
{
    fprintf(stderr, "\n-----BEGIN TABLE-----\n");
    symbol_stack* stack = scope_stack;
    while (stack != NULL)
    {
        khash_t(id)* h = stack->kh_table_top;
        for (khiter_t k = kh_begin(h); k != kh_end(h); ++k)
        {
            if (kh_exist(h, k))
            {
                print_symrec(kh_value(h, k));
            }
        }
        fprintf(stderr, "--------------------\n");
        stack = stack->previous;
    }
}

void 
initialize_stack()
{
    scope_stack = (symbol_stack*) malloc(sizeof(symbol_stack));
    scope_stack->kh_table_top = kh_init(id);
    scope_stack->previous = NULL;
}

void 
create_scope()
{
    symbol_stack* curr = scope_stack;
    scope_stack = (symbol_stack*) malloc(sizeof(symbol_stack));
    scope_stack->kh_table_top = kh_init(id);
    scope_stack->previous = curr;
}

void
add_to_scope(symrec* record)
{
    int return_value;
    khiter_t k;
    khash_t(id)* h = scope_stack->kh_table_top;
    // put id in hashtable
    k = kh_put(id, h, record->id, &return_value);
    symrec* val;
    // associate id to record. It looks weird, but it expands to an lvalue
    kh_value(h, k) = record;
}

// "destructor" for scopes
void 
delete_scope()
{
    symbol_stack* curr = scope_stack;
    khash_t(id)* h = curr->kh_table_top;
    scope_stack = scope_stack->previous;
    for (khiter_t k = kh_begin(h); k != kh_end(h); ++k)
    {
		if (kh_exist(h, k)) 
        {
            darray_symrec* arrp = kh_value(h, k)->parameter_list;
            // You can't iterate over a null parameter list.
            if (arrp == NULL) continue;
            // Free the symrecs in the parameter list
            for (size_t i = 0; i < arrp->length; i++)
            {
                // Parameter symrecs can't have more parameters. If that
                // weren't the case we'd need to free recursively.
                free(darray_get(arrp, i));
            }
            free(arrp->base);
            // Free the parameter list itself
            free(kh_value(h, k)->parameter_list); 
            // Free the symrec
            free(kh_value(h, k));
        }
    }
    free(curr->kh_table_top);
    free(curr);
}

symrec* 
search_in_current_scope(int id)
{
    khiter_t k;
    khash_t(id)* h = scope_stack->kh_table_top;
    k = kh_get(id, h, id);
    // If there is no k 
    if (k == kh_end(h))
    {
        return NULL;
    }
    else
    {
        return kh_val(h, k);
    }
}

symrec* 
search_in_any_scope(int id)
{
    khiter_t k;
    symbol_stack* stack = scope_stack;
    while (stack != NULL)
    {
        khash_t(id)* h = stack->kh_table_top;
        k = kh_get(id, h, id);
        // If there is no k 
        if (k != kh_end(h))
        {
            return kh_val(h, k);
        }
        stack = stack->previous;
    }
    return NULL;
}

symrec* 
proc_declare(int id)
{
    if (search_in_current_scope(id) != NULL)
        return NULL;
    symrec* s = (symrec*) malloc(sizeof(symrec));
    s->id = id;
    s->parameter_list = NULL;
    s->spec = PROCEDURE;
    s->type = T_INVALID; // procs return no type, so they can't be rvalues.
    add_to_scope(s);
    return s;
}

symrec* 
const_declare(int id, 
              YYSTYPE value, 
              int type)
{
    if (search_in_current_scope(id) != NULL)
        return NULL;
    symrec* s = (symrec*) malloc(sizeof(symrec));
    s->id = id;
    s->parameter_list = NULL;
    s->spec = CONST;
    if (type == T_INT_CONST)
    {
        s->type = T_INTEGER;
    }
    else if (type == T_REAL_CONST)
    {
        s->type = T_REAL;
    }
    else if (type == T_BOOLEAN_CONST)
    {
        s->type = T_BOOLEAN;
    }
    add_to_scope(s);
    return s;
}

symrec*
var_declare(int id, int type)
{
    if (search_in_current_scope(id) != NULL)
        return NULL;
    symrec* s = (symrec*) malloc(sizeof(symrec));
    s->id = id;
    s->parameter_list = NULL;
    s->spec = VAR;
    s->type = type;
    add_to_scope(s);
    return s;
}
//
// BIOHAZARD: This copies pointers to the parameter hashtables. This isn't supposed to be
// a problem since we never copy parameter symrecs and then delete them.
//
symrec* 
copy_symrec(symrec* base)
{
    symrec* s = (symrec*) malloc(sizeof(symrec));
    memcpy(s, base, sizeof(symrec));
    return s;
}

// returns 0 if id is not an lvalue (only vars are lvalues)
int 
check_lvalue(int id)
{
    return 0;
}

// returns 0 if id is not an rvalue 
int 
check_rvalue(int id)
{
    return 0;
}

khash_t(id)* 
operator_plus_assign(khash_t(id)* h1,
                     khash_t(id)* h2,
                     int* ret)
{
    int voidret;
    *ret = -1;
    for (khiter_t k = kh_begin(h2); k != kh_end(h2); ++k)
    {
		if (kh_exist(h2, k))
        {
            symrec* s = kh_value(h2, k);
            // put key on h1 and set value
            int key = kh_key(h2, k); 
            khiter_t k1 = kh_get(id, h1, key);
            if (k1 != kh_end(h1))
            {
                // Key already present! Overwrite and notify who that fucker is 
                *ret = kh_value(h1, k1)->id;
            }
            k1 = kh_put(id, h1, key, &voidret);
            kh_value(h1, k1) = s;
        }
    }
    return h1;
}

khash_t(id)* 
merge_hashes(khash_t(id)* h1, 
             khash_t(id)* h2)
{
    int* ret;
    khash_t(id)* h = kh_init(id);
    for (khiter_t k = kh_begin(h1); k != kh_end(h1); ++k)
    {
		if (kh_exist(h1, k))
        {
            symrec* s = kh_value(h1, k);
            // Free the symrecs in the parameter list
            kh_put(id, h, k, ret);
            if (ret == 0) return NULL;
        }
    }
    for (khiter_t k = kh_begin(h2); k != kh_end(h2); ++k)
    {
		if (kh_exist(h2, k))
        {
            symrec* s = kh_value(h2, k);
            // Free the symrecs in the parameter list
        }
    }
    return h;
}
