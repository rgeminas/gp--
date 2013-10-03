#ifndef __SYMREC_H
#define __SYMREC_H

#include "scope/khash.h"
#include "parser/base_parser.h"
// Forward declaration.
struct symrec;

// "declaration" of hashtable type
KHASH_MAP_INIT_INT(id, struct symrec*)

typedef enum { CONST, VAR, PROCEDURE, PARAM } species;

// type is one of: T_INTEGER, T_REAL, T_BOOL, 0 (for procedure declarations)
// parameter list is only there in procedures with non-void signatures
// id is retrieved from yylval. value is only set if spec == CONST.
typedef struct symrec
{
    species spec; 
    size_t id; // 2ndary token
    int type;
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

void add_to_scope(symrec*);

void initialize_stack();

void create_scope();

void delete_scope();

symrec* search_in_current_scope(int id);

void const_declare(int id, YYSTYPE value, int type);
#endif