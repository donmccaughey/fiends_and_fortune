#ifndef FNF_BASE_FAIL_H_INCLUDED
#define FNF_BASE_FAIL_H_INCLUDED


#define NOT_IMPLEMENTED() \
    fail("%s:%i: Function `%s()` not implemented.", __FILE__, __LINE__, __FUNCTION__)

#define UNEXPECTED_CASE(CASE) \
    fail("%s:%i: Unexpected case `%s` in %s().", __FILE__, __LINE__, #CASE, __FUNCTION__)


void
fail(char const *format, ...);


#endif
