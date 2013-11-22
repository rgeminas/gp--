#include <stdlib.h>

#include "wml.h"
#include "scope/type.h"
#include "lexer/lexer.h"

extern char** secondary_tokens;

/*

BIOHAZARD: THIS CODE LEAKS MEMORY LIKE LADIES IN PORN
LEAK SQUIRTING FLUIDS. LEAVE A BUCKET BELOW THE COMPUTER
TO COLLECT THE LEAKING MEMORY, THIS ISN'T GOING TO BE FIXED
BEFORE TUESDAY. - Gemig

*/

#define OPERATOR_S(op) wml_operators[operator_table[op]]

char* wml_operators[num_operators] = { "ADD", "SUB", "MUL", "DIV", "IDIV", "REM", "AND", "OR", "NOT", "EQ", "NE", "LE", "GE", "LT", "GT"};

size_t constant_counter = 0;
size_t procedure_counter = 1;
size_t* variable_stack;
size_t label_counter = 0;

char* 
wml_start_if(expression e,
             char* if_block_code)
{
    char* r = format("%s\n"
                     "TJUMP_FW_W $%d\n"
                     "%s\n"
                     "LABEL $%d\n", e.code, label_counter, if_block_code, label_counter);
    label_counter++;
    return r;
}

char* 
wml_start_if_else(expression e,
                  char* if_block_code,
                  char* else_block_code)
{
    char* r = format("%s\n"
                     "TJUMP_FW_W $%d\n"
                     "%s\n"
                     "JUMP_FW_W $%d\n"
                     "LABEL $%d\n"
                     "%s\n"
                     "LABEL $%d", e.code, label_counter, if_block_code, label_counter + 1, label_counter, else_block_code, label_counter + 1);
    label_counter += 2;
    return r;
}

char* 
wml_generate_expression_code(expression e1,
                             expression e2)
{
    return format("%s\n%s\n%s\n", e1.code, e2.code, OPERATOR_S(e2.operation)); 
}

char* 
wml_generate_program(size_t id,
                     char* body)
{
    return body;
}

char*
wml_generate_block_body(char* const_part,
                        char* var_part,
                        char* proc_part,
                        char* block_code)
{
    char* r = format("%s\n%s\n%s\n\EXTERN FUNCTION MAIN 0 0\n\FUNCVARS %d\n\%s\nRETURN", 
        const_part, var_part, proc_part, *variable_stack, block_code);
    return r;
}

char* 
wml_concatenate_procedures(char* c1, 
                           char* c2)
{
    char* r = format("%s\n%s", c1, c2);
    return r;
}

char*
wml_concatenate_parameter(char* c1,
                          char* c2)
{
    // This is NOT the pascal convention. Arguments are passed
    // From right to left so we reverse the order
    return format("%s\n%s\n", c2, c1);
}

char*
wml_concatenate_constants(char* c1,
                          char* c2)
{
    char* r = format("%s\n%s", c1, c2);
    return r;
}

char* 
wml_concatenate_variables(char* c1,
                          char* c2)
{
    char* r = format("%s\n%s", c1, c2);
    return r;
}

char*
wml_concatenate_statements(char* c1,
                           char* c2)
{
    char* r = format("%s\n%s", c1, c2);
    return r;
}

char*
wml_generate_constant_def(symrec* s)
{
    s->bp_off = constant_counter++;
    if (s->type == T_INTEGER)
        return format("CONST INT %d", s->value.int_const);
    // Unsupported by the assembler.
    if (s->type == T_REAL)
        return format("CONST REAL %f", s->value.real_const);
    // I have no idea what the format for this is. Guessing time.
    if (s->type == T_BOOLEAN)
        return format("CONST BOOLEAN %d", s->value.int_const);
}

char*
wml_generate_variable_def(darray(symrec)* vars)
{
    char* base = "", tmp;
    for(int i = 0; i < vars->length; i++)
    {
        symrec* s = darray_get(vars, i); 
        s->bp_off = *variable_stack;
        ++*variable_stack;
    }
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
    return format("%s\n"
                  "STORE_VAR_S %d\n", e.code, s->bp_off);
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
    if (!strcmp(secondary_tokens[s->id], "print"))
    {
        return format("%s\nCALL_LIB_S 2 5\n", param_push_code);
    }
    else
    {
        return format("%s\nCALL_S %d\n", param_push_code, s->bp_off);
    }

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
    return format("%s\n%s\n%s\n", lhs.code, rhs.code, OPERATOR_S(operator_id));
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
    constant_counter++;
    return format("CONST INT %d\n"
                  "LOAD_CONST_S %d", 
                   c,
                   constant_counter - 1);
}

char*
wml_declare_real_const_and_use_code(float c)
{
    // Not implemented because the assembler used doesn't support it
    exit(1);
    return "";
}

char*
wml_declare_bool_const_and_use_code(int c)
{
    constant_counter++;
    return format("CONST BOOLEAN %d\n"
                  "LOAD_CONST_S %d", 
                  c,
                  constant_counter - 1);
}

char*
wml_get_accessor_code(symrec* s)
{
    if (s->spec == CONST)
    {
        return format("LOAD_CONST_S %d\n", s->bp_off);
    }
    else if (s->spec == VAR)
    {
        return format("LOAD_VAR_S %d\n", s->bp_off);
    }
    else return "";
}

void 
init_wml()
{
    // Ugly, and arbitrary. FUCK IT IT'S ALMOST OVER ANYWAY
    variable_stack = (size_t*) malloc(sizeof(size_t) * 15);
    *variable_stack = 0;

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
