#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer/token.h"
#include "lexer/lexical_state_machine.h"

#ifndef __LEXER_H
#define __LEXER_H

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
            // If execute_transition fails, we went through more characters than needed.
            // Go back one position in the file, and return the code of the current state.
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

#endif // __LEXER_H
