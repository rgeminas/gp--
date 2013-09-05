%{

/*
 To generate the parser run "$ bison base_parser.y"
*/

#include <lexer/lexer.h>
#include <lexer/token.h>

extern int yylex(void);
extern int yyerror(char*);
/*
%output  "base_parser.c"
%defines "base_parser.h"
*/
%}

%union {
    int int_const;
    float real_const;
    size_t id_secondary;
}

%token T_EOF                  0
%token T_ID                   1
%token T_INT_CONST            2
%token T_REAL_CONST           3
%token T_BOOLEAN_CONST        4
%token T_PROGRAM              5
%token T_PROCEDURE            6
%token T_BEGIN                7
%token T_END                  8
%token T_IF                   9
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
%token T_REWIND_ONE          254
%token T_INVALID             256

/*
Notation: opt_XXX: [ XXX ]
          star_XXX: { XXX }
          plus_XXX: XXX { XXX }
*/
%right T_THEN T_ELSE

%%
input: T_PROGRAM T_ID T_SEMICOLON block_body T_PERIOD
;

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
                   | T_ID T_EQ T_REAL_CONST T_SEMICOLON
                   | T_ID T_EQ T_BOOLEAN_CONST T_SEMICOLON
;

variable_definition_part: T_VAR plus_variable_definition
;

plus_variable_definition: variable_definition 
                        | variable_definition plus_variable_definition
;

variable_definition: variable_group T_SEMICOLON
;

variable_group: T_ID star_comma_id T_COLON type
;

star_comma_id: 
             | T_COMMA T_ID star_comma_id
;

type: T_INTEGER 
    | T_REAL 
    | T_BOOLEAN
;

procedure_definition: T_PROCEDURE T_ID procedure_block T_SEMICOLON
;

procedure_block: opt_brc_formal_parameter_list_brc T_SEMICOLON block_body
;

opt_brc_formal_parameter_list_brc: 
                                 | T_LBRACKET formal_parameter_list T_RBRACKET
;

formal_parameter_list: parameter_definition star_smc_parameter_definition
;

star_smc_parameter_definition: 
                             | T_SEMICOLON parameter_definition star_smc_parameter_definition
;

parameter_definition: variable_group
;

statement: assignment_statement
         | procedure_statement
         | if_statement
         | while_statement
         | compound_statement
         |
;

assignment_statement: variable_access T_ASSIGN expression
;

procedure_statement: T_ID opt_brc_actual_parameter_list_brc
;

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
            | T_IF expression T_THEN statement T_ELSE statement
;

while_statement: T_WHILE expression T_DO statement
;

compound_statement: T_BEGIN statement star_comma_statement T_END
;

star_comma_statement: 
                    | T_COMMA statement star_comma_statement
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
;

constant: T_INT_CONST
        | T_REAL_CONST
        | T_BOOLEAN_CONST
        | variable_access
;
%%
