#include "scope/type.h"
#include "scope/symrec.h"

#include "parser/base_parser.h"

int return_by_expression[num_types][num_types][num_operators];
int coercible_types[num_types][num_types];
int type_table[MAX_TOKEN_ID]; 
int operator_table[MAX_TOKEN_ID]; 
//
// Initializes invalid values for all tokens that aren't types/operators
// Then, set valid values for usage with EXPR_RETURN.
// Then fill the tables, which is annoying as fuck.
//
void initialize_type_tables()
{
    int system_types[num_types] = { T_INTEGER, T_REAL, T_BOOLEAN, T_INVALID };
    int system_operators[num_operators] = { T_PLUS, T_MINUS, T_TIMES, T_DIVIDE, T_DIV, T_MOD, T_AND, T_OR, T_NOT, T_EQ, T_DIF, T_LEQ, T_GEQ, T_LT, T_GT};

    for (int i = 0; i < MAX_TOKEN_ID; i++)
    {
        type_table[i] = -1;
    }
    for (int i = 0; i < MAX_TOKEN_ID; i++)
    {
        operator_table[i] = -1;
    }

    for (int i = 0; i < num_types; i++)
    {
        type_table[system_types[i]] = i;
    }
    for (int i = 0; i < num_operators; i++)
    {
        operator_table[system_operators[i]] = i;
    }

    COMPATIBLE(T_INTEGER, T_INTEGER) = 1;  
    COMPATIBLE(T_INTEGER, T_REAL) = 0;
    COMPATIBLE(T_INTEGER, T_BOOLEAN) = 0;  

    COMPATIBLE(T_REAL, T_INTEGER) = 1;  
    COMPATIBLE(T_REAL, T_REAL) = 1;
    COMPATIBLE(T_REAL, T_BOOLEAN) = 0;

    COMPATIBLE(T_BOOLEAN, T_INTEGER) = 0;
    COMPATIBLE(T_BOOLEAN, T_REAL) = 0;
    COMPATIBLE(T_BOOLEAN, T_BOOLEAN) = 1;  
    // Defining hardcoded returns for every expression ever.

    // +
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_PLUS) = T_INTEGER;
    EXPR_RETURN(T_INTEGER, T_REAL, T_PLUS) = T_REAL;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_PLUS) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_PLUS) = T_REAL;
    EXPR_RETURN(T_REAL, T_REAL, T_PLUS) = T_REAL;
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_PLUS) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_PLUS) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_PLUS) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_PLUS) = T_INVALID;

    // -
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_MINUS) = T_INTEGER;
    EXPR_RETURN(T_INTEGER, T_REAL, T_MINUS) = T_REAL;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_MINUS) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_MINUS) = T_REAL;
    EXPR_RETURN(T_REAL, T_REAL, T_MINUS) = T_REAL;
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_MINUS) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_MINUS) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_MINUS) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_MINUS) = T_INVALID;
    
    // *
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_TIMES) = T_INTEGER;
    EXPR_RETURN(T_INTEGER, T_REAL, T_TIMES) = T_REAL;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_TIMES) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_TIMES) = T_REAL;
    EXPR_RETURN(T_REAL, T_REAL, T_TIMES) = T_REAL;
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_TIMES) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_TIMES) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_TIMES) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_TIMES) = T_INVALID;

    // /
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_DIVIDE) = T_REAL;
    EXPR_RETURN(T_INTEGER, T_REAL, T_DIVIDE) = T_REAL;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_DIVIDE) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_DIVIDE) = T_REAL;
    EXPR_RETURN(T_REAL, T_REAL, T_DIVIDE) = T_REAL;
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_DIVIDE) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_DIVIDE) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_DIVIDE) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_DIVIDE) = T_INVALID;

    // div
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_DIV) = T_INTEGER;
    EXPR_RETURN(T_INTEGER, T_REAL, T_DIV) = T_INTEGER;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_DIV) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_DIV) = T_INTEGER;
    EXPR_RETURN(T_REAL, T_REAL, T_DIV) = T_INTEGER;
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_DIV) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_DIV) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_DIV) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_DIV) = T_INVALID;
    
    // mod
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_MOD) = T_INTEGER;
    EXPR_RETURN(T_INTEGER, T_REAL, T_MOD) = T_REAL;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_MOD) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_MOD) = T_REAL;
    EXPR_RETURN(T_REAL, T_REAL, T_MOD) = T_REAL;
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_MOD) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_MOD) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_MOD) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_MOD) = T_INVALID;

    // and
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_AND) =  T_INVALID;
    EXPR_RETURN(T_INTEGER, T_REAL, T_AND) =  T_INVALID;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_AND) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_AND) = T_INVALID;
    EXPR_RETURN(T_REAL, T_REAL, T_AND) = T_INVALID;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_AND) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_AND) = T_BOOLEAN;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_AND) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_AND) = T_INVALID;

    // or 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_OR) =  T_INVALID;
    EXPR_RETURN(T_INTEGER, T_REAL, T_OR) =  T_INVALID;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_OR) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_OR) = T_INVALID;
    EXPR_RETURN(T_REAL, T_REAL, T_OR) = T_INVALID;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_OR) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_OR) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_OR) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_OR) = T_BOOLEAN;

    // not 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_NOT) =  T_INVALID;
    EXPR_RETURN(T_INTEGER, T_REAL, T_NOT) =  T_INVALID;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_NOT) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_NOT) = T_INVALID;
    EXPR_RETURN(T_REAL, T_REAL, T_NOT) = T_INVALID;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_NOT) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_NOT) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_NOT) = T_INVALID;
    EXPR_RETURN(T_INVALID, T_BOOLEAN, T_NOT) = T_BOOLEAN;

    // = 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_EQ) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_REAL, T_EQ) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_EQ) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_EQ) = T_BOOLEAN;
    EXPR_RETURN(T_REAL, T_REAL, T_EQ) = T_BOOLEAN;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_EQ) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_EQ) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_EQ) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_EQ) = T_INVALID;

    // <> 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_DIF) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_REAL, T_DIF) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_DIF) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_DIF) = T_BOOLEAN;
    EXPR_RETURN(T_REAL, T_REAL, T_DIF) = T_BOOLEAN;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_DIF) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_DIF) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_DIF) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_DIF) = T_INVALID;

    // < 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_LT) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_REAL, T_LT) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_LT) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_LT) = T_BOOLEAN;
    EXPR_RETURN(T_REAL, T_REAL, T_LT) = T_BOOLEAN;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_LT) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_LT) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_LT) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_LT) = T_INVALID;

    // <= 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_LEQ) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_REAL, T_LEQ) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_LEQ) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_LEQ) = T_BOOLEAN;
    EXPR_RETURN(T_REAL, T_REAL, T_LEQ) = T_BOOLEAN;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_LEQ) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_LEQ) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_LEQ) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_LEQ) = T_INVALID;
    
    // >
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_GT) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_REAL, T_GT) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_GT) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_GT) = T_BOOLEAN;
    EXPR_RETURN(T_REAL, T_REAL, T_GT) = T_BOOLEAN;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_GT) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_GT) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_GT) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_GT) = T_INVALID;

    // >= 
    EXPR_RETURN(T_INTEGER, T_INTEGER, T_GEQ) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_REAL, T_GEQ) =  T_BOOLEAN;
    EXPR_RETURN(T_INTEGER, T_BOOLEAN, T_GEQ) = T_INVALID;

    EXPR_RETURN(T_REAL, T_INTEGER, T_GEQ) = T_BOOLEAN;
    EXPR_RETURN(T_REAL, T_REAL, T_GEQ) = T_BOOLEAN;  
    EXPR_RETURN(T_REAL, T_BOOLEAN, T_GEQ) = T_INVALID;
    
    EXPR_RETURN(T_BOOLEAN, T_INTEGER, T_GEQ) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_REAL, T_GEQ) = T_INVALID;
    EXPR_RETURN(T_BOOLEAN, T_BOOLEAN, T_GEQ) = T_INVALID;
}

