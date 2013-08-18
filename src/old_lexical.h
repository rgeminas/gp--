#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "lexical_state_machine.h"

#ifndef __OLD_LEXICAL_H
#define __OLD_LEXICAL_H

inline bool
is_alphanum(char a) 
{
    return (((a) >= 'a' && (a) <= 'z') || ((a) >= 'A' && (a) <= 'Z') || ((a) <= '9' && (a) >= '0'));
}

/*
 Sets pos to the first non-whitespace and non-comment comment character. 
 Returns 0 in normal operation, returns an error code in case the input
 terminates with an unmatched comment mark.
*/

int
skip_non_tokens(char** pos)
{
    int inside_brace_comments = false;
    int inside_bracket_comments = false;

    while ((**pos == ' ' || **pos == '\r' || **pos == '\n' || **pos == '\t' || **pos == '\v' || inside_bracket_comments || inside_brace_comments) && **pos != '\0')
    {
        if (inside_bracket_comments == false)
        {
            if(**pos == '}') inside_brace_comments = false;
        }
        if (inside_brace_comments == false)
        {
            if(**pos == '*' && *(*pos+1) == ')')
            {
                ++*pos;
                inside_bracket_comments = false;
            }
        }
        ++*pos;
        if (inside_bracket_comments == false)
        {
            if(**pos == '{') inside_brace_comments = true;
        }
        if (inside_brace_comments == false)
        {
            if(**pos == '(' && *(*pos+1) == '*')
            {
                ++*pos;
                inside_bracket_comments = true;
            }
        }
    }
    if (inside_bracket_comments || inside_brace_comments)
    {
        fprintf(stderr, "Error - Unterminated comment.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/*
 Returns the appropriate token code for a string - its reserved word code
 or T_ID in case it's not a reserved word. This is case-sensitive.

 TODO: Stick a hashtable in here!
*/
int 
identify_string(const char* buffer)
{
    if (!strcmp(buffer,"program")) 
        return T_PROGRAM;
    else if (!strcmp(buffer,"procedure")) 
        return T_PROCEDURE;
    else if (!strcmp(buffer,"begin")) 
        return T_BEGIN;
    else if (!strcmp(buffer,"end")) 
        return T_END;
    else if (!strcmp(buffer,"if")) 
        return T_IF;
    else if (!strcmp(buffer,"then")) 
        return T_THEN;
    else if (!strcmp(buffer,"else")) 
        return T_ELSE;
    else if (!strcmp(buffer,"while")) 
        return T_WHILE;
    else if (!strcmp(buffer,"do")) 
        return T_DO;
    else if (!strcmp(buffer,"div")) 
        return T_DIV;
    else if (!strcmp(buffer,"mod")) 
        return T_MOD;
    else if (!strcmp(buffer,"and")) 
        return T_AND;
    else if (!strcmp(buffer,"or")) 
        return T_OR;
    else if (!strcmp(buffer,"not")) 
        return T_NOT;
    else if (!strcmp(buffer,"var"))
        return T_VAR;
    else if (!strcmp(buffer,"integer"))
        return T_INTEGER;
    else if (!strcmp(buffer,"real"))
        return T_REAL;
    else if (!strcmp(buffer,"boolean"))
        return T_BOOLEAN;
    else if (!strcmp(buffer,"const"))
        return T_CONST;
    else if (!strcmp(buffer,"repeat"))
        return T_REPEAT;
    else if (!strcmp(buffer,"until"))
        return T_UNTIL;
    else if (!strcmp(buffer,"true"))
        return T_BOOLEAN_CONST;
    else if (!strcmp(buffer,"false"))
        return T_BOOLEAN_CONST;
    else 
        return T_ID; 
}

/*
 Returns one of the token codes in token.h and forwards *pos to the first character
 after the token. 

 If the read value is not completely defined by its type (as in the case of an 
 integer constant, or a variable with a name), return_ptr will have a pointer to
 a structure (whose referenced type can be inferred by the return value of next_token (such
 as int* for T_INT_CONST or char** for T_ID) and ptr_size will point to the amount of
 memory allocated.

 Variable names are assumed to have 16 or less characters and any more than that
 will raise an error.
*/
int 
next_token(char** pos, 
          void** return_ptr,         
          size_t* ptr_size)
{
    size_t ptr_size_value = 0;
    void* return_ptr_value = NULL;
    skip_non_tokens(pos);
    int offset = 0;
    //parse int
    if (**pos <= '9' && **pos >= '0')
    {
        int return_value_int = 0;
        return_value_int += **pos - '0';
        while (*++*pos <= '9' && **pos >= '0')
        {
            return_value_int *= 10;
            return_value_int += **pos - '0';    
        }
        //parse real (SP)
        if (**pos == '.')
        {
            float return_value_real = (float) return_value_int;
            float division_factor = 1;
            while (*++*pos <= '9' && **pos >= '0')
            {
                division_factor *= 10;
                return_value_real += ((float) (**pos - '0')) / division_factor;
            }
            *return_ptr = (void*) &return_value_real;
            ptr_size_value = 1;
            ptr_size = &ptr_size_value;
            return T_REAL_CONST;
        }
        *return_ptr = (void*) &return_value_int;
        ptr_size_value = 1;
        ptr_size = &ptr_size_value;
        return T_INT_CONST;
    }
    //parse word (reserved or not)
    else if(is_alphanum(**pos))
    {
        size_t string_size = 1;
        char* return_string;
        char* buffer = (char*) malloc((MAX_IDENTIFIER_LENGTH + 1) * sizeof(char));
        char* aux_buffer = buffer;

        *aux_buffer = **pos;
        // If it reads 16 characters and there still are alphanums to parse,
        // an error is output;
        while(is_alphanum(*++*pos) && string_size < MAX_IDENTIFIER_LENGTH)
        {
            string_size++;
            *++aux_buffer = **pos;
        }
        if(is_alphanum(**pos) && string_size == MAX_IDENTIFIER_LENGTH)
        {
            free(buffer);
            return T_INVALID;
        }
        *++aux_buffer = '\0';
        string_size++;
        return_string = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(return_string,buffer);
        free(buffer);
        *return_ptr = (void*) return_string;
        ptr_size = &string_size;
        return identify_string(return_string);
    }
    else if (**pos == ':')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_ASSIGN;
        }
        else
        {
            return T_COLON;
        }
    }
    else if (**pos == '+')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_PLUS_EQ;
        }
        else
        {
            return T_PLUS;
        }
    }
    else if (**pos == '-')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_MINUS_EQ;
        }
        else
        {
            return T_MINUS;
        }
    }
    else if (**pos == '*')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_TIMES_EQ;
        }
        else
        {
            return T_TIMES;
        }
    }
    else if (**pos == '/')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_DIVIDE_EQ;
        }
        else

        {
            return T_DIVIDE;
        }
    }
    else if (**pos == '.')
    {
        ++*pos;
        if (**pos == '.')
        {
            ++*pos;
            return T_DOUBLE_PERIOD;
        }
        else
        {
            return T_PERIOD;
        }
    }
    else if (**pos == '>')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_GEQ;
        }
        else
        {
            return T_GT;
        }
    }
    else if (**pos == '<')
    {
        ++*pos;
        if (**pos == '=')
        {
            ++*pos;
            return T_LEQ;
        }
        if (**pos == '>')
        {
            ++*pos;
            return T_DIF;
        }
        else
        {
            return T_LT;
        }
    }
    else if (**pos == '=')
    {
        ++*pos;
        return T_EQ;
    }
    else if (**pos == ';')
    {
        ++*pos;
        return T_SEMICOLON;
    }
    else if (**pos == ',')
    {
        ++*pos;
        return T_COMMA;
    }
    else if (**pos == '(')
    {
        ++*pos;
        return T_LBRACKET;
    }
    else if (**pos == ')')
    {
        ++*pos;
        return T_RBRACKET;
    }
    else if(**pos == '\0')
    {
        return T_EOF;
    }
    return T_INVALID;
}

#endif
