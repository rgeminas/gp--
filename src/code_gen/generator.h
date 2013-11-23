#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "scope/type.h"
#include "scope/symrec.h"

typedef struct generator
{
    char* (*start_if)(expression, char*);
    char* (*start_if_else)(expression, char*, char*);
    char* (*generate_expression_code)(expression, expression);
    char* (*generate_program)(size_t id, char* body);
    char* (*generate_block_body)(char*, char*, char*, char*);
    char* (*concatenate_procedures)(char*, char*);
    char* (*concatenate_parameter)(char*, char*);
    char* (*concatenate_constants)(char*, char*);
    char* (*concatenate_variables)(char*, char*);
    char* (*concatenate_statements)(char*, char*);
    char* (*generate_constant_def)(symrec*);
    char* (*generate_variable_def)(darray(symrec)*);
    char* (*generate_procedure_def)(symrec*);
    char* (*generate_assignment)(symrec*, expression);
    char* (*call_void_procedure)(symrec*);
    char* (*call_procedure)(symrec*, char*);
    char* (*start_while)(expression, char*);
    char* (*generate_relational_expression_code)(expression, expression, size_t); 
    char* (*generate_signed_expression_code)(expression, expression, size_t); 
    char* (*generate_signed_single_expression_code)(expression, size_t); 
    char* (*generate_not_expression_code)(expression);
    char* (*declare_int_const_and_use_code)(int);
    char* (*declare_real_const_and_use_code)(float);
    char* (*declare_bool_const_and_use_code)(int);
    char* (*get_accessor_code)(symrec*);
    void (*start_procedure)(symrec*);

} generator;

extern generator global_gen;

#endif
