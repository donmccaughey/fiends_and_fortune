#ifndef FNF_BASE_UNREACHABLE_H_INCLUDED
#define FNF_BASE_UNREACHABLE_H_INCLUDED


#include "xstdio.h"


void
unreachable(char const *format, ...) XMALLOC_PRINTF(1, 2);


#endif
