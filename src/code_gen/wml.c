#include <stdlib.h>
#include "wml.h"

char* 
wml_factor_op_expr(expression e,
                   expression e2)
{
    return "";
}

char* 
wml_start_if(expression e,
             char* if_block_code)
{
    return "";
}

char* 
wml_start_if_else(expression e,
                  char* if_block_code,
                  char* else_block_code)
{
    return "";
}

void 
initwml()
{
    global_gen.start_if = &wml_start_if;
    global_gen.start_if_else = &wml_start_if_else;
}
