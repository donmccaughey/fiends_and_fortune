#ifndef ASTR_H_INCLUDED
#define ASTR_H_INCLUDED


#include <stdarg.h>
#include <stdbool.h>


#if defined(__clang__) || defined(__GNUC__)
#define ASTR_FORMAT(...) __attribute__((__format__(__printf__, __VA_ARGS__)))
#else
#define ASTR_FORMAT(...)
#endif


char *
astr_cat_f(char *s, char const *format, ...) ASTR_FORMAT(2, 3);

char *
astr_cat_f_va(char *s, char const *format, va_list args) ASTR_FORMAT(2, 0);

char *
astr_centered_f(int width, char const *format, ...) ASTR_FORMAT(2, 3);

char *
astr_centered_f_va(int width, char const *format, va_list args)
    ASTR_FORMAT(2, 0);

int
astr_cmp(char const *s1, char const *s2);

char *
astr_dup(const char *s);

char *
astr_empty(void);

bool
astr_eq(char const *s1, char const *s2);

char *
astr_f(char const *format, ...) ASTR_FORMAT(1, 2);

char *
astr_f_va(char const *format, va_list args) ASTR_FORMAT(1, 0);

bool
astr_is_empty(char const *s);

int
astr_len(char const *s);

int
astr_len_f(char const *format, ...) ASTR_FORMAT(1, 2);

int
astr_len_f_va(char const *format, va_list args) ASTR_FORMAT(1, 0);


#endif
