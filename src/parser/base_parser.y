%{

#include <stdio.h>
#include "scope/khash.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "scope/symrec.h"
#include "code_gen/generator.h"

extern int yylex(void);
extern int yyerror(char*);
extern char** secondary_tokens;

%}
%code requires 
{
#include "scope/type.h"
#include <stddef.h>
}

%union 
{
    int int_const;
    float real_const;
    size_t id;
    struct symrec* record;
    struct expression expr;
    char* block_code;
}

%token T_EOF                   0
%token T_ID                    1
%token T_INT_CONST             2
%token T_REAL_CONST            3
%token T_BOOLEAN_CONST         4
%token T_PROGRAM               5
%token T_PROCEDURE             6
%token T_BEGIN                 7
%token T_END                   8
%token T_IF                    9
%token T_THEN                 10
%token T_ELSE                 11
%token T_WHILE                12
%token T_DO                   13
%token T_ASSIGN               14
%token T_PLUS                 15
%token T_MINUS                16
%token T_TIMES                17
%token T_DIVIDE               18
%token T_DIV                  19
%token T_MOD                  20
%token T_PLUS_EQ              21
%token T_MINUS_EQ             22
%token T_TIMES_EQ             23
%token T_AND                  24
%token T_OR                   25
%token T_NOT                  26
%token T_DIVIDE_EQ            27
%token T_VAR                  28
%token T_INTEGER              29
%token T_REAL                 30
%token T_BOOLEAN              31
%token T_COLON                32
%token T_SEMICOLON            33
%token T_COMMA                34
%token T_PERIOD               35
%token T_DOUBLE_PERIOD        36
%token T_LBRACKET             37
%token T_RBRACKET             38
%token T_CONST                39
%token T_REPEAT               40
%token T_UNTIL                41
%token T_EQ                   42
%token T_DIF                  43
%token T_LT                   44
%token T_LEQ                  45
%token T_GT                   46
%token T_GEQ                  47
%token T_FOR                  48
%token T_TO                   49

/* Non-token tokens */
%token T_REWIND_ONE          254
%token T_INVALID             256
/*
Notation: opt_XXX: [ XXX ]
          star_XXX: { XXX }
          plus_XXX: XXX { XXX }

force_XXX are always reduced from empty, and are used to execute semantic actions,
such as allocating memory space;

*/

%no-lines

%right T_THEN T_ELSE

%type<record> formal_parameter_list opt_brc_formal_parameter_list_brc variable_group star_comma_id parameter_definition star_smc_parameter_definition opt_brc_actual_parameter_list_brc actual_parameter_list star_comma_actual_parameter variable_access
%type<expr> star_adding_operator_term star_multiplying_operator_factor factor expression simple_expression actual_parameter term constant
%type<id> T_ID type adding_operator relational_operator multiplying_operator sign_operator
%type<int_const> T_INT_CONST T_BOOLEAN_CONST
%type<real_const> T_REAL_CONST
%type<block_code> input block_body opt_constant_definition_part opt_variable_definition_part star_procedure_definition constant_definition_part variable_definition_part procedure_definition plus_constant_definition constant_definition plus_variable_definition variable_definition assignment_statement procedure_statement if_statement while_statement compound_statement star_comma_statement statement procedure_block 
%%
input: T_PROGRAM T_ID T_SEMICOLON force_initialization block_body T_PERIOD 
{   
    $$ = global_gen.generate_program($2, $5);
    // Print to console
    fprintf(stdout, "%s", $$);
    // deallocate last scope
    delete_scope(); 
};


force_initialization: 
{ 
    initialize_stack();
    initialize_type_tables();
};

block_body: opt_constant_definition_part opt_variable_definition_part star_procedure_definition compound_statement
{
    $$ = global_gen.generate_block_body($1, $2, $3, $4);
}
;

opt_constant_definition_part: 
{
    $$ = "";
}
                            | constant_definition_part
{
    $$ = $1;
};

opt_variable_definition_part: 
{
    $$ = "";
}
                            | variable_definition_part  
{
    $$ = $1;
};

star_procedure_definition: 
{
    $$ = "";
}
                         | procedure_definition star_procedure_definition
{
    $$ = global_gen.concatenate_procedures($1, $2);
}
;

constant_definition_part: T_CONST plus_constant_definition
{
    $$ = $2;
}
;

plus_constant_definition: constant_definition 
{
    // Void function
    $$ = $1;
}
                        | constant_definition plus_constant_definition
{
    $$ = global_gen.concatenate_constants($1, $2);
}
;

constant_definition: T_ID T_EQ T_INT_CONST T_SEMICOLON 
{ 
    YYSTYPE u;
    u.int_const = $3;
    symrec* s = const_declare($1, u, T_INT_CONST);
    if (s == NULL) 
    {
        fprintf(stderr, "ERROR: Redefinition of symbol '%s' ", secondary_tokens[$1]);
        print_location();
        YYERROR; 
    }
    $$ = global_gen.generate_constant_def(s);
} 
                   | T_ID T_EQ T_REAL_CONST T_SEMICOLON 
{
    YYSTYPE u;
    u.real_const = $3;
    symrec* s = const_declare($1, u, T_REAL_CONST);
    if (s == NULL) 
    {
        fprintf(stderr, "ERROR: Redefinition of symbol '%s' ", secondary_tokens[$1]);
        print_location();
        YYERROR; 
    }
    $$ = global_gen.generate_constant_def(s);
} 
                   | T_ID T_EQ T_BOOLEAN_CONST T_SEMICOLON 
{
    YYSTYPE u;
    u.int_const = $3;
    symrec* s = const_declare($1, u, T_BOOLEAN_CONST);
    if (s == NULL) 
    {
        fprintf(stderr, "ERROR: Redefinition of symbol '%s' ", secondary_tokens[$1]);
        print_location();
        YYERROR; 
    }
    $$ = global_gen.generate_constant_def(s);
};

variable_definition_part: T_VAR plus_variable_definition
{
    // Void function
    //$$ = global_gen.set_variable_ids($2);
    $$ = $2;
}
;

plus_variable_definition: variable_definition 
{
    $$ = $1;
}
                        | variable_definition plus_variable_definition
{
    $$ = global_gen.concatenate_variables($1, $2);
}
;

variable_definition: variable_group T_SEMICOLON
{
    for (size_t i = 0; i < $1->parameter_list->length; i++)
    {
        symrec* s = darray_get($1->parameter_list, i);
        s->spec = VAR;
        add_to_scope(s);
    }
    $$ = global_gen.generate_variable_def($1->parameter_list);
    free($1->parameter_list);
    free($1);
};

variable_group: T_ID star_comma_id T_COLON type
{
    int ret;
    $$ = $2; 
    symrec* k = darray_find_id($$->parameter_list, $1);
	if (k == NULL)
    {
        // Create symrec for the parameter
        symrec* s = (symrec*) malloc(sizeof(symrec));
        s->spec = PARAM;
        s->id = $1;
        s->parameter_list = NULL;

        darray_push_front(symrec)($$->parameter_list, s);
        // Iterate over symrecs, set type.
        for (size_t i = 0; i < $$->parameter_list->length; i++)
        {
            darray_get($$->parameter_list, i)->type = $4;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Multiple declarations of the same symbol '%s' ", secondary_tokens[$1]);
        print_location();
        YYERROR;
    }
};

star_comma_id: 
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->parameter_list = darray_init(symrec)();
}
             | T_COMMA T_ID star_comma_id
{
    int ret;
    $$ = $3; 
    symrec* k = darray_find_id($$->parameter_list, $2);
	if (k == NULL)
    {
        // Create symrec for the parameter
        symrec* s = (symrec*) malloc(sizeof(symrec));
        s->spec = PARAM;
        s->id = $2;
        s->parameter_list = NULL;

        darray_push_front(symrec)($$->parameter_list, s);
        // Type is still undefined!
    }
    else
    {
        fprintf(stderr, "ERROR: Multiple declarations of the same symbol '%s' ", secondary_tokens[$2]);
        print_location();
        YYERROR;
    }
};

type: T_INTEGER 
{ 
    $$ = T_INTEGER; 
}
    | T_REAL 
{ 
    $$ = T_REAL; 
}
    | T_BOOLEAN 
{ 
    $$ = T_BOOLEAN; 
};

procedure_definition: procedure_block block_body T_SEMICOLON 
{

    delete_scope(); 
};

procedure_block: T_PROCEDURE T_ID opt_brc_formal_parameter_list_brc T_SEMICOLON 
{
    symrec* s = proc_declare($2);
    darray_symrec* arr = $3->parameter_list;
    // Copy parameter list to the actual symrec
    s->parameter_list = $3->parameter_list;

    free($3);  // the parameter list is still going to be there, don't worry

    create_scope();

    // add stuff from parameter_list to the newly-created scope so the underlying function can access it
    for (size_t i = 0; i < arr->length; i++)
    {
        add_to_scope(copy_symrec(darray_get(arr, i)));
    }
    $$ = global_gen.generate_procedure_def(s);
};

opt_brc_formal_parameter_list_brc: 
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->parameter_list = darray_init(symrec)();
}
                                 | T_LBRACKET formal_parameter_list T_RBRACKET
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->parameter_list = $2->parameter_list;
    free($2);
};

formal_parameter_list: parameter_definition star_smc_parameter_definition
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->type = 0; 
    // do not add to scope yet
    // MERGE parameter_definition into star_smc_parameter_definition and copy into formal_parameter_list
    int ret = 1;
    for (size_t i = 0; i < $2->parameter_list->length; i++)
    {
        symrec* k = darray_find_id($1->parameter_list, darray_get($2->parameter_list, i)->id);
        if (k != NULL)
        {
            ret = 0;
            fprintf(stderr, "ERROR: Definition of multiple parameters with the same identifier %s ", 
                    secondary_tokens[darray_get($2->parameter_list, i)->id]);
            print_location();
            break;
        }
        darray_push_back(symrec)($1->parameter_list, darray_get($2->parameter_list, i));
    }
    $$->parameter_list = $1->parameter_list;
    free($2->parameter_list);
    
    if (!ret) YYERROR;
};

star_smc_parameter_definition:
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->type = 0; 
    $$->parameter_list = darray_init(symrec)();
} 
                             | T_SEMICOLON parameter_definition star_smc_parameter_definition
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->type = 0; 
    // do not add to scope yet
    // MERGE parameter_definition into star_smc_parameter_definition and copy into formal_parameter_list
    int ret = 1;
    for (size_t i = 0; i < $3->parameter_list->length; i++)
    {
        symrec* k = darray_find_id($2->parameter_list, darray_get($3->parameter_list, i)->id);
        if (k != NULL)
        {
            ret = 0;
            break;
        }
        darray_push_back(symrec)($2->parameter_list, darray_get($3->parameter_list, i));
    }
    $$->parameter_list = $2->parameter_list;
    if (!ret)
    {
        fprintf(stderr, "\nERROR: Multiple parameters with the same name in the definition of a procedure ");
        print_location();
    }
    free($3->parameter_list);
    if (!ret) YYERROR;
};

parameter_definition: variable_group
{
    $$ = $1;
};

statement: 
{
    $$ = "";
}
         | procedure_statement
{
    $$ = $1;
}
         | if_statement
{
    $$ = $1;
}
         | while_statement
{
    $$ = $1;
}
         | compound_statement
{
    $$ = $1;
}
         | assignment_statement
{
    $$ = $1;
}
;

assignment_statement: variable_access T_ASSIGN expression
{
    symrec* s = $1;
    if (s->spec != VAR && s->spec != PARAM)
    {
        fprintf(stderr, "ERROR: Assignment to non-lvalue '%s' ", secondary_tokens[$1->id]);
        print_location();
        YYERROR;
    }
    if (!COMPATIBLE(s->type, $3.term_type))
    {
        fprintf(stderr, "ERROR: Cannot coerce expression into variable '%s' ", secondary_tokens[$1->id]);
        print_location();
        YYERROR;
    }
    $$ = global_gen.generate_assignment(s, $3);
};

procedure_statement: T_ID opt_brc_actual_parameter_list_brc
{
    /*
    BIOHAZARD: Our scope analysis didn't use expression arrays for passing parameters,
    using symrecs instead. We pay for this now by having to inject pushing code into a
    symrec. Fuck.
    */
    symrec* proc_symrec = search_in_any_scope($1);
    if (proc_symrec == NULL || proc_symrec->spec != PROCEDURE)
    {
        fprintf(stderr, "ERROR: Undefined procedure '%s' ", secondary_tokens[$1]);
        print_location();
        YYERROR;
    }
    if (proc_symrec->parameter_list->length == 0)
    {
        if ($2->parameter_list->length > 0)
        {
            fprintf(stderr, "ERROR: Procedure '%s' has void signature, but is being passed parameters ",
                    secondary_tokens[$1]);
            print_location();
            YYERROR;
        }
        $$ = global_gen.call_void_procedure(proc_symrec);
    }
    else
    {
        if ($2->parameter_list->length == 0)
        {
            fprintf(stderr, "ERROR: Procedure '%s' does not have void signature, but is being passed no parameters ",
                    secondary_tokens[$1]);
            print_location();
            YYERROR;
        }
        if ($2->parameter_list->length != proc_symrec->parameter_list->length)
        {
            fprintf(stderr, "ERROR: Procedure call for '%s' is not passing the right amount of parameters, expected '%ld', got '%ld' ", 
                    secondary_tokens[$1], proc_symrec->parameter_list->length, $2->parameter_list->length);
            print_location();
            YYERROR;
        }
        for (size_t i = 0; i < proc_symrec->parameter_list->length; i++)
        {
            if (!COMPATIBLE(darray_get(proc_symrec->parameter_list, i)->type, darray_get($2->parameter_list, i)->type))
            {
                fprintf(stderr, "ERROR: Cannot coerce expression into parameter '%s' ", secondary_tokens[darray_get(proc_symrec->parameter_list, i)->id]);
                print_location();
                YYERROR;
            }
        }
        $$ = global_gen.call_procedure(proc_symrec, $2->code);
    }
};

opt_brc_actual_parameter_list_brc:
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->parameter_list = darray_init(symrec)();
    $$->code = "";
}
                                 | T_LBRACKET actual_parameter_list T_RBRACKET
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->parameter_list = $2->parameter_list;
    $$->code = $2->code;
};

actual_parameter_list: actual_parameter star_comma_actual_parameter
{
    $$ = $2;
    symrec* s = (symrec*) malloc(sizeof(symrec));
    s->id = -1;
    s->spec = PARAM;
    s->type = $1.term_type;
    darray_push_front(symrec)($$->parameter_list, s);
    $$->code = global_gen.concatenate_parameter($1.code, $2->code);
};

star_comma_actual_parameter: 
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->parameter_list = darray_init(symrec)();
    $$->code = "";
}
                           | T_COMMA actual_parameter star_comma_actual_parameter
{
    $$ = $3;
    symrec* s = (symrec*) malloc(sizeof(symrec));
    s->id = -1;
    s->spec = PARAM;
    s->type = $2.term_type;
    darray_push_front(symrec)($$->parameter_list, s);
    $$->code = global_gen.concatenate_parameter($2.code, $3->code);
};

actual_parameter: expression { $$ = $1; }
;

if_statement: T_IF expression T_THEN statement
{
    if ($2.term_type != T_BOOLEAN) 
    {
        fprintf(stderr, "ERROR: Expression does not have boolean type ");
        print_location();
        YYERROR;
    }
    $$ = global_gen.start_if($2, $4);
}
            | T_IF expression T_THEN statement T_ELSE statement
{
    if ($2.term_type != T_BOOLEAN)
    {
        fprintf(stderr, "ERROR: Expression does not have boolean type ");
        print_location();
        YYERROR;
    }
    $$ = global_gen.start_if_else($2, $4, $6);
};

while_statement: T_WHILE expression T_DO statement
{
    if ($2.term_type != T_BOOLEAN)
    {
        fprintf(stderr, "ERROR: Expression does not have boolean type ");
        print_location();
        YYERROR;
    }
    $$ = global_gen.start_while($2, $4);
};


compound_statement: T_BEGIN statement star_comma_statement T_END
{
    $$ = global_gen.concatenate_statements($2, $3);
}
;

star_comma_statement: 
{
    $$ = "";
}
                    | T_SEMICOLON statement star_comma_statement
{
    $$ = global_gen.concatenate_statements($2, $3);
}
;

expression: simple_expression relational_operator simple_expression
{
        //printf("SIMPLE_EXPRESSION\n");
        //printf("\n\n%d REL %d, OP IS %d\n\n", $1.term_type, $2, $3.term_type);
        $$.term_type = EXPR_RETURN($1.term_type, $3.term_type, $2);
        //printf("\n\nRESULT IS %d", $$.term_type);
        //print_location();
        if ($$.term_type == T_INVALID) 
        {
            fprintf(stderr, "ERROR: Unable to operate operands ");
            print_location();
            YYERROR;
        }
        $$.code = global_gen.generate_relational_expression_code($1, $3, $2);
}
          | simple_expression
{
    $$ = $1;
}
;

relational_operator: T_LT { $$ = T_LT; }
                   | T_EQ { $$ = T_EQ; }
                   | T_GT { $$ = T_GT; }
                   | T_LEQ { $$ = T_LEQ; }
                   | T_DIF { $$ = T_DIF; }
                   | T_GEQ { $$ = T_GEQ; }
;

simple_expression: sign_operator term star_adding_operator_term
{
    if($3.operation != T_INVALID && $3.operation != T_INVALID)
    {
        $$.term_type = EXPR_RETURN($2.term_type, $3.term_type, $3.operation);
        if ($$.term_type == T_INVALID)
        {
            fprintf(stderr, "ERROR: Unable to operate operands ");
            print_location();
            YYERROR;
        }
        $$.code = global_gen.generate_signed_expression_code($2, $3, $1);
    }
    else
    {
        $$ = $2;
    }
}
                 | term star_adding_operator_term
{ 
    if($2.operation != T_INVALID && $2.operation != T_INVALID)
    {
        $$.term_type = EXPR_RETURN($1.term_type, $2.term_type, $2.operation);
        if ($$.term_type == T_INVALID)
        {
            fprintf(stderr, "ERROR: Unable to operate operands ");
            print_location();
            YYERROR;
        }
        $$.code = global_gen.generate_expression_code($1, $2);
    }
    else
    {
        $$ = $1;
    }
};

sign_operator: T_PLUS { $$ = T_PLUS; }
             | T_MINUS { $$ = T_MINUS; }
;

star_adding_operator_term: 
{
    $$.operation = T_INVALID;
    $$.term_type = T_INVALID;
}
                         | adding_operator term star_adding_operator_term
{
    if ($3.operation != T_INVALID && $3.term_type != T_INVALID)
    {
        $$.operation = $1;
        $$.term_type = EXPR_RETURN($2.term_type, $3.term_type, $3.operation);
        if ($$.term_type == T_INVALID)
        {
            fprintf(stderr, "ERROR: Unable to operate operands ");
            print_location();
            YYERROR;
        }
        $$.code = global_gen.generate_expression_code($2, $3);
    }
    else
    {
        $$ = $2;
        $$.operation = $1;
    }
};

adding_operator: T_PLUS
{
    $$ = T_PLUS;
}
               | T_MINUS
{
    $$ = T_MINUS;
}
               | T_OR
{
    $$ = T_OR;
};

term: factor star_multiplying_operator_factor
{
    if ($2.operation != T_INVALID && $2.term_type != T_INVALID)
    {
        //printf("TERM\n");
        //printf("\n\n%d OPERATED WITH %d, OP IS %d\n\n", $1.term_type, $2.term_type, $2.operation);
        $$.term_type = EXPR_RETURN($1.term_type, $2.term_type, $2.operation);
        //printf("\n\nRESULT IS %d\n\n", $$.term_type);
        //print_location();
        if ($$.term_type == T_INVALID)
        {
            fprintf(stderr, "ERROR: Unable to operate operands ");
            print_location();
            YYERROR;
        }
        $$.code = global_gen.generate_expression_code($1, $2);
    }
    else
    {
        $$ = $1;
    }
};

star_multiplying_operator_factor:
{
    $$.operation = T_INVALID;
    $$.term_type = T_INVALID;
}
                                | multiplying_operator factor star_multiplying_operator_factor
{
    $$.operation = $1;
    if ($3.operation != T_INVALID && $3.term_type != T_INVALID)
    {
        $$.operation = $1;
        $$.term_type = EXPR_RETURN($2.term_type, $3.term_type, $3.operation);
        if ($$.term_type == T_INVALID)
        {
            fprintf(stderr, "ERROR: Unable to operate operands ");
            print_location();
            YYERROR;
        }
        $$.code = global_gen.generate_expression_code($2, $3);
    }
    else
    {
        $$ = $2;
        $$.operation = $1;

    }
};

multiplying_operator: T_TIMES { $$ = T_TIMES; } 
                    | T_DIV { $$ = T_DIV; }
                    | T_MOD { $$ = T_MOD; }
                    | T_AND { $$ = T_AND; }
                    | T_DIVIDE { $$ = T_DIVIDE; }
                    ;

factor: constant
{
    $$ = $1;
}
      | T_LBRACKET expression T_RBRACKET
{
    $$ = $2;
}
      | T_NOT factor
{
    $$.term_type = EXPR_RETURN(T_INVALID, $2.term_type, T_NOT);
    if ($$.term_type == T_INVALID)
    {
        fprintf(stderr, "ERROR: Unable to execute operation ");
        print_location();
        YYERROR;
    }
    $$.code = global_gen.generate_not_expression_code($2);
};

variable_access: T_ID 
{ 
    // The symbol must exist! Checking its species is done at a later stage.
    symrec* s = search_in_any_scope($1);
    if (s == NULL)
    {
        fprintf(stderr, "ERROR: Use of undeclared identifier '%s' ", secondary_tokens[$1]);
        print_location();
        YYERROR;
    }
    $$ = s;
};

constant: T_INT_CONST
{
    $$.term_type = T_INTEGER;
    $$.code = global_gen.declare_int_const_and_use_code($1);
}
        | T_REAL_CONST
{
    $$.term_type = T_REAL;
    // This is never going to happen
    $$.code = global_gen.declare_real_const_and_use_code($1);
} 
        | T_BOOLEAN_CONST
{
    $$.term_type = T_BOOLEAN;
    $$.code = global_gen.declare_bool_const_and_use_code($1);
} 
        | variable_access
{
    // Pass on the type as an expression
    $$.term_type = $1->type;
    $$.code = global_gen.get_accessor_code($1);
};
%%
