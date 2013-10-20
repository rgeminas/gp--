#include "lexer/token.h"
#include "parser/base_parser.h"

#ifndef __TYPE_H
#define __TYPE_H

#define MAX_TOKEN_ID 259 

#define COMPATIBLE(lhs, rhs) coercible_types[type_table[lhs]][type_table[rhs]]
#define EXPR_RETURN(lt, rt, op) return_by_expression[type_table[lt]][type_table[rt]][operator_table[op]]
#define num_types 4
#define num_operators 15

struct expression; 

typedef struct expression 
{
    int operation;
    int term_type;
} expression;

int type_table[MAX_TOKEN_ID]; 
// *smirk*
int operator_table[MAX_TOKEN_ID]; 

// TODO: Make this into a fucking pointer already.
int return_by_expression[num_types][num_types][num_operators];
int coercible_types[num_types][num_types];

void initialize_type_tables(void);

#endif // __TYPE_H
