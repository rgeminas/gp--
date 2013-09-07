#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef _MSC_VER
  #include <strings.h>
#endif

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "lexer/lexical_state_machine.h"
#include "parser/base_parser.h"
#include "portability/portability.h"

extern PAIR tokens[];
extern PAIR keywords[];

STATE_MACHINE* _sm;
STATE_MACHINE* _wssm;
FILE* _f = NULL;

extern int yytype;
extern YYSTYPE yylval;

char** secondary_tokens = (char**) malloc(sizeof(char*));
size_t sec_alloc = 1;
size_t sec_i = 0;

TOKEN
skip_nontokens_file(FILE* file,
                    STATE_MACHINE* sm)
{
    long int penultimate_position = 0;
    long int last_position = ftell(file);
    // Do transitions until execute_transition returns an error (no valid transition)
    while(1)
    {
        
        int c = tolower(fgetc(file));
        if(c == EOF)
        {
            if(sm->current_state->return_token == T_INVALID)
            {
                TOKEN t = {T_INVALID, NULL};
                fprintf(stderr, "Syntax error on file position %d.\n", last_position);
                return t;
            }
            else
            {
                TOKEN t = {T_EOF, NULL};
                return t;
            }
        }
        if(execute_transition(sm, c))
        {
            // If execute_transition fails, we went through more characters than needed.
            // Go back one position in the file, and return the code of the current state.
            fseek(file, last_position, SEEK_SET);
            break;
        }
        penultimate_position = last_position;
        last_position = ftell(file);
    }
    // Set sm back to its initial state so it can skip more whitespace later.
    int ret = sm->current_state->return_token;
    if(ret == T_INVALID)
    {
        fprintf(stderr, "Syntax error on file position %d.\n", last_position);
    }
    if(ret == T_REWIND_ONE)
    {
        fseek(file, penultimate_position, SEEK_SET);
    }
    reset_machine(sm);
    TOKEN t = {ret, NULL};
    return t;
}

TOKEN
next_token_file(FILE* file, 
                STATE_MACHINE* sm, 
                STATE_MACHINE* wsssm)
{
    TOKEN return_skip = skip_nontokens_file(file, wsssm);
    if(return_skip.type == T_INVALID)
    {
        return return_skip;
    }
    char* buffer = (char*) malloc(sizeof(char));
    size_t allocated = 1;
    size_t i = 0;

    int c = tolower(fgetc(file));
    if(c == EOF)
    {
        TOKEN t = {T_EOF, NULL};
        return t;
    }

    long int last_position = ftell(file);
    // Do transitions until execute_transition returns an error (no valid transition)
    // This assumes that your file ends on a newline.
    while(1) 
    {
        if(execute_transition(sm, c))
        {
            // If execute_transition fails, we went through more characters than needed
            // Go back in the file, and return the code of the current state.
            --i;
            fseek(file, last_position, SEEK_SET);
            break;
        }

        if(i + 1 >= allocated)
        {
            allocated = allocated << 1;
            buffer = (char*) realloc(buffer, allocated * sizeof(char));
        }
        buffer[i] = sm->current_state->state;
        last_position = ftell(file);
        c = tolower(fgetc(file));
        ++i;
    }
    // Null-terminate string.
    buffer[++i] = 0;
    // Set sm back to its initial state so it can tokenize another string.
    int ret = sm->current_state->return_token;
    if(ret == T_INVALID)
    {
        fprintf(stderr, "Syntax error on file position %d.\n", last_position);
    }
    reset_machine(sm);
    TOKEN t = {ret, buffer};
    return t;
}

int 
yylex(void)
{
    if(_f == NULL) return T_INVALID;
    TOKEN p = next_token_file(_f, _sm, _wssm);
    if(p.type == T_EOF)
    {
        return 0;
    }
    if(p.type == T_ID)
    {
        if(sec_alloc <= sec_i)
        {
            sec_alloc = sec_alloc << 1;
            secondary_tokens = (char**) realloc(secondary_tokens, sec_alloc * sizeof(char*));
        }
        size_t i;
        for(i=0; i<sec_i; ++i)
        {
            if(!stricmp(secondary_tokens[i], p.token_value))
            {
                break;
            }
        }
        // If we went through the full loop without a match, add a new secondary token.
        if(i == sec_i)
        {
            secondary_tokens[sec_i++] = p.token_value;
        }
        // Set secondary token value.
        yylval.id_secondary = i;
    }
    else if(p.type == T_INT_CONST)
    {
        yylval.int_const = atoi((char*) p.token_value);
    }
    else if(p.type == T_REAL_CONST)
    {
        yylval.real_const = atof((char*) p.token_value);
    }
    else if(p.type == T_BOOLEAN_CONST)
    {
        if(stricmp((char*) p.token_value, "true"))
        {
            yylval.int_const = 1;
        }
        else
        {
            yylval.int_const = 0;
        }
    }
    else // 2ndary token unneeded
    {
        //yylval.id_name = p.token_value;
    }
    //printf("%d ", p.type);
    return p.type;
}

int
yyerror(char* a)
{
    puts("Parsing error");
    puts(a);
    puts("");
    return 0;
}
