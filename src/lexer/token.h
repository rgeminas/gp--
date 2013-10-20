/*
 Definition of tokens returned by the lexical analysis compiler pass.
*/

#include <stdlib.h>
// Include bison generated tokens.
#include "parser/base_parser.h"

#ifndef __TOKEN_H
#define __TOKEN_H


#define TOKEN_TYPE int

typedef struct PAIR
{
    char* token;
    TOKEN_TYPE type;
} PAIR;

extern PAIR tokens[];

extern PAIR keywords[];

#endif // __TOKEN_H
