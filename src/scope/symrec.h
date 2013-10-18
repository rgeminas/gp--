#ifndef __SYMREC_H
#define __SYMREC_H

#include "scope/khash.h"
#include "parser/base_parser.h"
// Forward declaration.
struct symrec;

// "declaration" of hashtable type
KHASH_MAP_INIT_INT(id, struct symrec*)

typedef enum { CONST, VAR, PROCEDURE, PARAM, PARAMLIST } species;

// type is one of: T_INTEGER, T_REAL, T_BOOL, 0 (for procedure declarations)
// parameter list is only there in procedures with non-void signatures
// id is retrieved from yylval. value is only set if spec == CONST.
typedef struct symrec
{
    species spec; 
    size_t id; // 2ndary token
    int type; // Among T_BOOLEAN, T_INTEGER, T_REAL. Bite me.
    YYSTYPE value; // This is never going to be a symrec
    khash_t(id)* parameter_list;
} symrec;

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

//
// If this were C++, this would be called
//    operator +=(...)
// Fortunately, this isn't C++.
//
khash_t(id)* 
operator_plus_assign(khash_t(id)* h1,
                     khash_t(id)* h2,
                     int* ret);
#endif
