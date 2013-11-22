#include <stdlib.h>
#include "wml.h"

char* 
wml_factor_op_expr(expression e,
                   expression e2)
{
    return "";
}

char* 
wml_start_if(expression e,
             char* if_block_code)
{
    return "";
}

char* 
wml_start_if_else(expression e,
                  char* if_block_code,
                  char* else_block_code)
{
    return "";
}

char* 
wml_generate_expression_code(expression e1,
                             expression e2)
{
    return "";
}

char* 
wml_generate_program(size_t id,
                     char* body)
{
    return "";
}

char*
wml_generate_block_body(char* const_part,
                        char* var_part,
                        char* proc_part,
                        char* block_code)
{
    return "";
}

char* 
wml_concatenate_procedures(char* c1, 
                           char* c2)
{
    return "";
}

char*
wml_concatenate_parameter(char* c1,
                          char* c2)
{
    return "";
}

char*
wml_concatenate_constants(char* c1,
                          char* c2)
{
    return "";
}

char* 
wml_concatenate_variables(char* c1,
                          char* c2)
{
    return "";
}

char*
wml_concatenate_statements(char* c1,
                           char* c2)
{
    return "";
}

char*
wml_generate_constant_def(symrec* s)
{
    return "";
}

char*
wml_generate_variable_def(darray(symrec)* vars)
{
    return "";
}

char*
wml_generate_procedure_def(symrec* s)
{
    return "";
}

char*
wml_generate_assignment(symrec* s, 
                        expression e)
{
    return "";
}

char*
wml_call_void_procedure(symrec* s)
{
    return "";
}

char*
wml_call_procedure(symrec* s, 
                   char* param_push_code)
{
    return "";
}

char*
wml_start_while(expression e,
                char* while_block)
{
    return "";
}

char*
wml_generate_relational_expression_code(expression lhs,
                                        expression rhs,
                                        size_t operator_id)
{
    return "";
}

char*
wml_generate_signed_expression_code(expression lhs,
                                    expression rhs,
                                    size_t sign)
{
    return "";
}

char* 
wml_generate_not_expression_code(expression e)
{
    return "";
}

char* 
wml_declare_int_const_and_use_code(int c)
{
    return "";
}

char*
wml_declare_real_const_and_use_code(float c)
{
    // Not implemented because the assembler used doesn't support it
    return "";
}

char*
wml_declare_bool_const_and_use_code(int c)
{
    return "";
}

char*
wml_get_accessor_code(symrec* s)
{
    return "";
}

void 
init_wml()
{
    global_gen.start_if = wml_start_if;
    global_gen.start_if_else = wml_start_if_else;
    global_gen.generate_expression_code = wml_generate_expression_code;
    global_gen.generate_program = wml_generate_program;
    global_gen.generate_block_body = wml_generate_block_body;
    global_gen.concatenate_procedures = wml_concatenate_procedures;
    global_gen.concatenate_parameter = wml_concatenate_parameter;
    global_gen.concatenate_constants = wml_concatenate_constants;
    global_gen.concatenate_variables = wml_concatenate_variables;
    global_gen.concatenate_statements = wml_concatenate_statements;
    global_gen.generate_constant_def = wml_generate_constant_def;
    global_gen.generate_variable_def = wml_generate_variable_def;
    global_gen.generate_procedure_def = wml_generate_procedure_def;
    global_gen.generate_assignment = wml_generate_assignment;
    global_gen.call_void_procedure = wml_call_void_procedure;
    global_gen.call_procedure = wml_call_procedure;
    global_gen.start_while = wml_start_while;
    global_gen.generate_relational_expression_code = wml_generate_relational_expression_code;
    global_gen.generate_signed_expression_code = wml_generate_signed_expression_code;
    global_gen.generate_not_expression_code = wml_generate_not_expression_code;
    global_gen.declare_int_const_and_use_code = wml_declare_int_const_and_use_code;
    global_gen.declare_real_const_and_use_code = wml_declare_real_const_and_use_code;
    global_gen.declare_bool_const_and_use_code = wml_declare_bool_const_and_use_code;
    global_gen.get_accessor_code = wml_get_accessor_code;
}
