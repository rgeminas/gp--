#ifndef __STRINGBUILDER_H
#define __STRINGBUILDER_H

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

// 
// Similar to sprintf, except it doesn't make you call it twice and think about how much memory you're going to use.
// Warning: This memory is malloc'ed. Free it later, stupid cuntfaced cocksucker.
//
char* format(const char* format, ...);
//
// Similar to strcat, except you don't have to worry about checking whether the destination is big enough to contain
// your string. This does nothing to s1 and s2, and it's your responsibility to free the result, stupid cocksucker.
//
char* append(char* s1, const char* s2);


#endif
