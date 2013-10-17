%{

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "scope/symrec.h"
#include "scope/khash.h"

extern int yylex(void);
extern int yyerror(char*);
extern char** secondary_tokens;

%}
%no_lines

%union 
{
    int int_const;
    float real_const;
    size_t id;
    struct symrec* record;
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

%right T_THEN T_ELSE

%type<record> formal_parameter_list opt_brc_formal_parameter_list_brc procedure_block variable_group star_comma_id parameter_definition star_smc_parameter_definition
%type<id> T_ID variable_access type
%type<int_const> T_INT_CONST T_BOOLEAN_CONST
%type<real_const> T_REAL_CONST

%%
input: T_PROGRAM T_ID T_SEMICOLON force_initialization block_body T_PERIOD { delete_scope(); } // deallocate last scope;
;

force_initialization: { initialize_stack(); } // empty syntactic marker;

block_body: opt_constant_definition_part opt_variable_definition_part star_procedure_definition compound_statement
;

opt_constant_definition_part: 
                            | constant_definition_part
;

opt_variable_definition_part: 
                            | variable_definition_part  
;

star_procedure_definition: 
                         | procedure_definition star_procedure_definition
;

constant_definition_part: T_CONST plus_constant_definition
;

plus_constant_definition: constant_definition 
                        | constant_definition plus_constant_definition
;

constant_definition: T_ID T_EQ T_INT_CONST T_SEMICOLON 
{ 
    YYSTYPE u;
    u.int_const = $3;
    if (const_declare($1, u, T_INT_CONST) == NULL) 
    {
        fprintf(stderr, "Redefinition of symbol '%s'.\n", secondary_tokens[$1]);
        YYERROR; 
    }
} 
                   | T_ID T_EQ T_REAL_CONST T_SEMICOLON 
{
    YYSTYPE u;
    u.real_const = $3;
    if (const_declare($1, u, T_REAL_CONST) == NULL) 
    {
        fprintf(stderr, "Redefinition of symbol '%s'.\n", secondary_tokens[$1]);
        YYERROR; 
    }
} 
                   | T_ID T_EQ T_BOOLEAN_CONST T_SEMICOLON 
{
    YYSTYPE u;
    u.int_const = $3;
    if (const_declare($1, u, T_BOOLEAN_CONST) == NULL) 
    {
        fprintf(stderr, "Redefinition of symbol '%s'.\n", secondary_tokens[$1]);
        YYERROR; 
    }
} 
;

variable_definition_part: T_VAR plus_variable_definition
;

plus_variable_definition: variable_definition 
                        | variable_definition plus_variable_definition
;

variable_definition: variable_group T_SEMICOLON
{
    for (khiter_t k = kh_begin($1->parameter_list); k != kh_end($1->parameter_list); ++k)
    {
        if (kh_exist($1->parameter_list, k))
        {
            symrec* s = kh_value($1->parameter_list, k);
            s->spec = VAR;
            add_to_scope(s);
        } 
    }
    free($1->parameter_list);
    free($1);
};

variable_group: T_ID star_comma_id T_COLON type
{
    int ret;
    $$ = $2; 
    khiter_t k = kh_get(id, $$->parameter_list, $1);
	if (k == kh_end($$->parameter_list))
    {
        k = kh_put(id, $$->parameter_list, $1, &ret);
        // Create symrec for the parameter!
        symrec* s = (symrec*) malloc(sizeof(symrec));
        s->spec = PARAM;
        s->id = $1;
        s->parameter_list = NULL;
        kh_value($$->parameter_list, k) = s;
        // Iterate over symrecs, set type.
        for (k = kh_begin($$->parameter_list); k != kh_end($$->parameter_list); ++k)
        {
            if (kh_exist($$->parameter_list, k))
            {
                kh_value($$->parameter_list, k)->type = $4;
            }
        }
    }
    else
    {
        fprintf(stderr, "Multiple declarations of the same symbol '%s'.\n", secondary_tokens[$1]);
        YYERROR;
    }
};

star_comma_id: 
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->parameter_list = kh_init(id);
}
             | T_COMMA T_ID star_comma_id
{
    int ret;
    $$ = $3; 
    khiter_t k = kh_get(id, $$->parameter_list, $2);
	if (k == kh_end($$->parameter_list))
    {
        k = kh_put(id, $$->parameter_list, $2, &ret);
        // Create symrec for the parameter!
        symrec* s = (symrec*) malloc(sizeof(symrec));
        s->spec = PARAM;
        s->id = $2;
        s->parameter_list = NULL;
        kh_value($$->parameter_list, k) = s;
        // Type is still undefined!
    }
    else
    {
        fprintf(stderr, "Multiple declarations of the same symbol '%s'.\n", secondary_tokens[$2]);
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
    $$ = proc_declare($2);
    khash_t(id)* hp = $3->parameter_list;
    // Copy parameter list to the actual symrec
    $$->parameter_list = $3->parameter_list;

    free($3);  // the parameter list is still going to be there, don't worry

    create_scope();

    // add stuff from parameter_list to the newly-created scope so the underlying function can access it
    for (khiter_t kp = kh_begin(hp); kp != kh_end(hp); ++kp)
    {
        if (kh_exist(hp, kp))
        {
            add_to_scope(copy_symrec(kh_value(hp, kp)));
        }
    }
    
};

opt_brc_formal_parameter_list_brc: 
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->parameter_list = kh_init(id);
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
    int ret;
    $$->parameter_list = operator_plus_assign($1->parameter_list, $2->parameter_list, &ret);
    free($2->parameter_list);
    if (!ret)
    {
        fprintf(stderr, "\nERROR: Definition of multiple variables with the same identifier.\n");
    }
    if (!ret) YYERROR;
};

star_smc_parameter_definition:
{

    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->type = 0; 
    $$->parameter_list = kh_init(id);
} 
                             | T_SEMICOLON parameter_definition star_smc_parameter_definition
{
    $$ = (symrec*) malloc(sizeof(symrec));
    $$->id = -1;
    $$->spec = PARAMLIST;
    $$->type = 0; 
    // do not add to scope yet
    // MERGE parameter_definition into star_smc_parameter_definition and copy into formal_parameter_list
    int ret;
    $$->parameter_list = operator_plus_assign($2->parameter_list, $3->parameter_list, &ret);
    if (!ret)
    {
        fprintf(stderr, "\nERROR: Multiple parameters with the same name in the definition of a procedure.\n");
    }
    free($3->parameter_list);
    if (!ret) YYERROR;
};

parameter_definition: variable_group
{
    $$ = $1;
};

statement: 
         | procedure_statement
         | if_statement
         | while_statement
         | compound_statement
         | assignment_statement
;

assignment_statement: variable_access T_ASSIGN expression
{
    symrec* s = search_in_any_scope($1);
    if (s->spec != VAR && s->spec != PARAM)
    {
        fprintf(stderr, "ERROR: Assignment to non-lvalue '%s'.\n", secondary_tokens[$1]);
        YYERROR;
    }
};

procedure_statement: T_ID opt_brc_actual_parameter_list_brc
{
    symrec* s = search_in_any_scope($1);
    if (s == NULL || s->spec != PROCEDURE)
    {
        fprintf(stderr, "ERROR: Undefined procedure '%s'\n", secondary_tokens[$1]);
        YYERROR;
    }
    // Check if signature matches once we get expression types straightened out!
};

opt_brc_actual_parameter_list_brc:
                                 | T_LBRACKET actual_parameter_list T_RBRACKET
;

actual_parameter_list: actual_parameter star_comma_actual_parameter
;

star_comma_actual_parameter: 
                           | T_COMMA actual_parameter star_comma_actual_parameter
;

actual_parameter: expression
;

if_statement: T_IF expression T_THEN statement
{
    // Check that expression has bool type
}
            | T_IF expression T_THEN statement T_ELSE statement
{
    // Check that expression has bool type
};

while_statement: T_WHILE expression T_DO statement
;

compound_statement: T_BEGIN statement star_comma_statement T_END
;

star_comma_statement: 
                    | T_SEMICOLON statement star_comma_statement
;

expression: simple_expression opt_relational_operator_simple_expression
;

opt_relational_operator_simple_expression: 
                                         | relational_operator simple_expression
;

relational_operator: T_LT
                   | T_EQ
                   | T_GT
                   | T_LEQ
                   | T_DIF
                   | T_GEQ
;

simple_expression: sign_operator term star_adding_operator_term
                 | term star_adding_operator_term
;

sign_operator: T_PLUS
             | T_MINUS
;

star_adding_operator_term: 
                         | adding_operator term star_adding_operator_term
;

adding_operator: T_PLUS
               | T_MINUS
               | T_OR
;

term: factor star_multiplying_operator_factor
;

star_multiplying_operator_factor: 
                                | multiplying_operator factor star_multiplying_operator_factor
;

multiplying_operator: T_TIMES 
                    | T_DIV
                    | T_MOD
                    | T_AND
                    | T_DIVIDE
;

factor: constant
      | T_LBRACKET expression T_RBRACKET
      | T_NOT factor
;

variable_access: T_ID 
{ 
    $$ = $1; 
    symrec* s = search_in_any_scope($1);
    if (s == NULL)
    {
        fprintf(stderr, "Use of undeclared identifier '%s'.\n", secondary_tokens[$1]);
        YYERROR;
    }
};

constant: T_INT_CONST
        | T_REAL_CONST
        | T_BOOLEAN_CONST
        | variable_access
;
%%
