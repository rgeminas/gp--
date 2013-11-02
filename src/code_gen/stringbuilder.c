#include "code_gen/stringbuilder.h"

char* format(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* dest = NULL;
    int memsize = vsnprintf(dest, 0, format, args); 

    dest = (char*) malloc(memsize + 1);
    vsnprintf(dest, memsize, format, args);
    dest[memsize] = 0;

    va_end(args);
    return dest;
}

char* append(char* s1, const char* s2)
{
    return format("%s%s", s1, s2);
}