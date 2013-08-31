#ifndef __LEXER_H
#define __LEXER_H

#include "state_machine/state_machine.h"
#include "lexer/token.h"
#include "lexer/lexical_state_machine.h"
#include "parser/base_parser.h"
#include <stdlib.h>
#include <stdio.h>

extern STATE_MACHINE* _sm; 
extern STATE_MACHINE* _wssm; 
extern FILE* _f;

TOKEN
skip_nontokens_file(FILE* file,
                    STATE_MACHINE* sm);

TOKEN
next_token_file(FILE* file, 
                STATE_MACHINE* sm, 
                STATE_MACHINE* wsssm);

/*
 Both these functions are needed for bison to 
 parse the generated lexemes.
*/
int 
yylex(void);

int
yyerror(char* a);

#endif // __LEXER_H
