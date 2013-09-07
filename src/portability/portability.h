/*
This header defines some macros to enable the program to compile under compilers that
don't define some of the functions used here in their standard libraries.
*/

#ifndef __PORTABILITY_H
#define __PORTABILITY_H

#if !_MSC_VER
#  define stricmp strcasecmp
#endif // !_MSC_VER

#endif // __PORTABILITY_H
