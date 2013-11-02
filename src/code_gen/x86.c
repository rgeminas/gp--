#include <stdio.h>
#include <string.h>
#include "code_gen/x86.h"

char* registers[] = { "eax", "ebx", "ecx", "edx", "esp", "ebp", "eip" };
char* fp_registers[] = { "st0", "st1", "st2" };
typedef enum { eax, ebx, ecx, edx, esp, ebp, eip } register_map;
typedef enum { st0, st1, st2 } fp_register_map;

int global_instruction_id;

fp_register_map get_another_fp_register(fp_register_map r1, fp_register_map r2)
{
    if (r1 > r2)
    {
        // This will go away once the function returns, remember
        fp_register_map temp;
        temp = r2;
        r2 = r1;
        r1 = temp;
    }
    switch (r1)
    {
    case st0:
        switch(r2)
        {
        case st1: return st2;
        case st2: return st1;
        }
    case st1: return st0;
    }
}
register_map get_another_register(register_map r1, register_map r2)
{
    if (r1 > r2)
    {
        // This will go away once the function returns, remember
        register_map temp;
        temp = r2;
        r2 = r1;
        r1 = temp;
    }
    switch (r1)
    {
    case eax:
        switch(r2)
        {
        case eax: return ebx;
        case ebx: return ecx;
        case ecx: return ebx;
        case edx: return ebx;
        }
    case ebx: return eax;
    case ecx: return eax;
    }
}

char*
x86_start_if(expression e, 
             char* if_block_code)
{
    char* code = format("cmp %s 0\n\
                         jne else_%d\n\
                         %s\n\
                         else_%d:\n",
                         registers[e.reg],
                         global_instruction_id,
                         if_block_code,
                         global_instruction_id);
    global_instruction_id += 1;
    free(if_block_code);

    return code;
}


char*
x86_start_if_else(expression e, 
                  char* if_block_code,
                  char* else_block_code)
{
    char* code = format("cmp %s 0\n\
                         jne else_%d\n\
                         %s\n\
                         jmp endif_%d\n\
                         else_%d:\n\
                         %s\n\
                         endif_%d:\n",
                         registers[e.reg],
                         global_instruction_id,
                         if_block_code,
                         global_instruction_id + 1,
                         global_instruction_id,
                         else_block_code,
                         global_instruction_id + 1);
    global_instruction_id += 2;
    free(if_block_code);
    free(else_block_code);

    return code;
}

void initx86()
{
    global_gen = (generator*) malloc(sizeof(generator));
    global_gen->start_if = &x86_start_if;
    global_gen->start_if_else = &x86_start_if_else;
}