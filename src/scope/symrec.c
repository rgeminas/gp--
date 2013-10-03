#include "scope/symrec.h"
#include "parser/base_parser.h"
#include <stdlib.h>

symbol_stack* scope_stack;

void initialize_stack()
{
    scope_stack = (symbol_stack*) malloc(sizeof(symbol_stack));
    scope_stack->kh_table_top = kh_init(id);
    scope_stack->previous = NULL;
}

void create_scope()
{
    symbol_stack* curr = scope_stack;
    scope_stack = (symbol_stack*) malloc(sizeof(symbol_stack));
    scope_stack->kh_table_top = kh_init(id);
    scope_stack->previous = curr;
}

void add_to_scope(symrec* record)
{
    int return_value;
    khiter_t k;
    khash_t(id)* h = scope_stack->kh_table_top;
    // put id in hashtable
    k = kh_put(id, h, record->id, &return_value);
    symrec* val;
    // associate id to record
    kh_value(h, k) = record;
}

// "destructor" for scopes
void delete_scope()
{
    symbol_stack* curr = scope_stack;
    khash_t(id)* h = curr->kh_table_top;
    scope_stack = scope_stack->previous;
    for (khiter_t k = kh_begin(h); k != kh_end(h); ++k)
    {
		if (kh_exist(h, k)) 
        {
            free(kh_value(h, k)->parameter_list); 
            free(kh_value(h, k));
        }
    }
    free(curr->kh_table_top);
    free(curr);
}

symrec* search_in_current_scope(int id)
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

void begin_block(void)
{
    create_scope();
}

void end_block(void)
{
    delete_scope();
}

void const_declare(int id, YYSTYPE value, int type)
{
    symrec* s = (symrec*) malloc(sizeof(symrec));
    s->id = id;
    s->parameter_list = NULL;
    s->spec = CONST;
    if(type == T_INT_CONST)
    {
        s->type = T_INTEGER;
    }
    else if(type == T_REAL_CONST)
    {
        s->type = T_REAL;
    }
    else if(type == T_BOOLEAN_CONST)
    {
        s->type = T_BOOLEAN;
    }
    add_to_scope(s);    
}

// returns 0 if id is not an lvalue
int check_lvalue(int id)
{
    return 0;
}

// returns 0 if id is not an rvalue
int check_rvalue(int id)
{
    return 0;
}