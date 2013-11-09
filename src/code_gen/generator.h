#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "scope/type.h"

typedef struct generator
{
    char* (*start_if)(expression, char*);
    char* (*start_if_else)(expression, char*, char*);
    void (*generate_expression_code)(expression, char*);
} generator;

extern generator global_gen;

#endif
