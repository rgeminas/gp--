#ifndef __WML_H
#define __WML_H

#include "generator.h"
#include "stringbuilder.h"

char* 
wml_factor_op_expr(expression e,
                   expression e2);

char* 
wml_start_if(expression e,
             char* if_block_code);

char* 
wml_start_if_else(expression e,
                  char* if_block_code,
                  char* else_block_code);

void init_wml();
#endif // __WML_H
