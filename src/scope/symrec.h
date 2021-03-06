#ifndef __SYMREC_H
#define __SYMREC_H

#include "scope/darray.h"
#include "scope/khash.h"
#include "parser/base_parser.h"
// Forward declaration.
struct symrec;
//struct darray_symrec;

// "declaration" of hashtable and darray types
KHASH_MAP_INIT_INT(id, struct symrec*)
DARRAY_DEFINE(symrec, struct symrec*)

typedef enum { CONST, VAR, PROCEDURE, PARAM, PARAMLIST } species;

typedef struct symrec
{
    species spec; 
    size_t id; // 2ndary token
    int type; // Among T_BOOLEAN, T_INTEGER, T_REAL. Bite me.
    YYSTYPE value; // This is never going to be a symrec
    struct darray_symrec* parameter_list; // Dynamic array of function parameters.
    char* code; // Mostly used to handle the pushing of expressions onto the stack;

    int reg;
    int bp_off;

} symrec;

symrec*
darray_find_id(darray_symrec* arr,
               int id);
// END OF DARRAY FUNCTIONS

// This struct is basically a backwards linked list of hashtables. I thought of 
// making a hashtable of hashtables, but decided this way was easier to work with
// and only marginally less performing since access is always from first to last.
typedef struct symbol_stack
{
    khash_t(id)* kh_table_top;
    struct symbol_stack* previous;
} symbol_stack;

void
print_symrec(symrec* s);

void
print_table();

symrec* 
copy_symrec(symrec*);

void 
add_to_scope(symrec*);

void 
initialize_stack();

void 
create_scope();

void 
delete_scope();

symrec* 
search_in_current_scope(int id);

symrec* 
search_in_any_scope(int id);

symrec*
const_declare(int id, 
              YYSTYPE value, 
              int type);

symrec*
var_declare(int id, 
            int type);

symrec* 
proc_declare(int id);

#endif
