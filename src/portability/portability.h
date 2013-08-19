#ifndef __PORTABILITY_H
#define __PORTABILITY_H

#if !_MSC_VER
#define stricmp strcasecmp
#endif // _MSC_VER && !_INTEL_COMPILER

#endif // __PORTABILITY_H
