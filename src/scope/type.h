#ifndef __TYPE_H
#define __TYPE_H

struct expression; 

typedef struct expression 
{
    int operation;
    int term_type;
} expression;

#include "parser/base_parser.h"
#include "lexer/token.h"

#define MAX_TOKEN_ID 259 

#define num_types 4
#define num_operators 15

#define COMPATIBLE(lhs, rhs) coercible_types[type_table[lhs]][type_table[rhs]]
#define EXPR_RETURN(lt, rt, op) return_by_expression[type_table[lt]][type_table[rt]][operator_table[op]]

// TODO: Make this into a fucking pointer already.
extern int return_by_expression[num_types][num_types][num_operators];
extern int coercible_types[num_types][num_types];
extern int type_table[MAX_TOKEN_ID]; 
extern int operator_table[MAX_TOKEN_ID]; 

void initialize_type_tables(void);

#endif // __TYPE_H
