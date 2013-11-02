#ifndef __X86_H
#define __X86_H

#include "generator.h"
#include "stringbuilder.h"

char*
x86_start_if(expression, 
             char*);

char*
x86_start_else(expression, 
               char*,
               char*);

void 
init_x86(void);

#endif
