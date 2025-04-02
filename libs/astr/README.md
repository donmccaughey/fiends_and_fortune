# astr

A C99 library for working with dynamically allocated strings.

[![builds.sr.ht status](https://builds.sr.ht/~donmcc/astr.svg)](https://builds.sr.ht/~donmcc/astr?)


## Overview

*astr* is a library of functions that make working with dynamically allocated
strings easier and less error-prone.  All functions in *astr* accept `NULL`
string pointers, treating them similarly to a zero-length string.  Reallocating
functions make string building easier by automatically resizing the string as
needed.  All allocations are checked; *astr* will call [`abort()`][1] if memory
allocation fails.

*astr* was inspired by the improvements that the [`asprintf()`][2] function
brings to string building.

[1]: http://man7.org/linux/man-pages/man3/abort.3.html
[2]: http://man7.org/linux/man-pages/man3/asprintf.3.html

### Naming Conventions

All functions in *astr* begin with the `astr_` prefix. Functions with `_f` in
their name take a `printf()` style format string.  Functions with `_cat` in
their name will add characters to the end of the given string, returning a new
string.  Functions that end with the suffix `_va` take variable arguments as a
`va_list` object from `stdarg.h`.

### Usage

All *astr* functions that return a `char *` value are returning a dynamically
allocated string; the caller is responsible for calling [`free()`][3] on the
returned pointer.  It is not necessary to check these returned values for
`NULL`; *astr* checks all memory allocations and will call `abort()` on memory
exhaustion.  If the first parameter of a function is a `char *` value, the
address of that string may be invalid after the function is called.

[3]: http://man7.org/linux/man-pages/man3/free.3.html


## Functions

    char *
    astr_dup(char const *s);

Duplicate a string; `s` may be `NULL`.

    char *
    astr_empty(void);

Allocate an empty string.

    char *
    astr_f(char const *format, ...);

Allocate a formatted string.

    char *
    astr_cat_f(char *s, char const *format, ...);

Append formatted characters to the end of the given string, returning the
new string; the address of the original string may be invalid after this
function is called.

    char *
    astr_centered_f(int width, char const *format, ...);

Allocate a formatted string centered in a string of the given width.  The
formatted string is _not_ truncated if it is wider than `width`.

    int
    astr_cmp(char const *s1, char const *s2);

Compare the bytes of two strings; `s1` and `s2` may be `NULL`.  The return
value is negative if `s1` is ordered before `s2`, positive if `s1` is ordered
after `s2` and zero if `s1` equals `s2`.  A `NULL` string pointer is ordered 
before all non-`NULL` string pointers.

    bool
    astr_eq(char const *s1, char const *s2);

Compare the bytes of two strings for equality; `s1` and `s2` may be `NULL`.

    bool
    astr_is_empty(char const *s);

Returns `true` if a string is `NULL` or zero-length.

    int
    astr_len(char const *s);

Calculate the length in bytes of a string; `s` may be `NULL`.

    int
    astr_len_f(char const *format, ...);

Calculate the length in bytes needed for the formatted string.

    char *
    astr_f_va(char const *format, va_list args);

    char *
    astr_cat_f_va(char *s, char const *format, va_list args);

    char *
    astr_centered_f_va(int width, char const *format, va_list args);

    int
    astr_len_f_va(char const *format, va_list args);

Variable argument list versions of the variadic functions above.  Useful for
building your own variadic functions.


## Macros

    ASTR_PRINTF(string_index, first_to_check)

This macro evaluates to the `format` function attribute for the [GCC][20] and
[Clang][21] compilers using the `printf` archetype.  For other compilers, this
macro evaluates to nothing.

The `string_index` macro parameter is the 1-based index of the modified
function's format string parameter.  The `first_to_check` macro parameter is the
1-based index of the start of the variable argument list in the modified
function; use zero when there is no variable argument list, such as for a
function with a `va_list` parameter.

[20]: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-format-function-attribute
[21]: https://clang.llvm.org/docs/AttributeReference.html#format


## Dependencies

Building requires [CMake 3.15][30] or later.

[30]: https://cmake.org


## Building from Repository Source

Clone the repository, generate the build system and build.

    git clone https://git.sr.ht/~donmcc/astr
    cd astr
    cmake -S . -B tmp
    cmake --build tmp
    cmake --install tmp

To run tests:

    cmake --build tmp --target all test

### Build Options

To build with the [AddressSanitizer][41] enabled, set the `ADDRESS-SANITIZER`
option to `ON`.

    cmake -S . -B tmp -DADDRESS_SANITIZER=ON

Set the `BUILD_TESTING` option to `OFF` to disable building tests.  (It's `ON`
by default.)

    cmake -S . -B tmp -DBUILD_TESTING=OFF

Set the `WALL` option to `ON` turns on [additional warnings][42] using the 
`-Wall` compiler flag and also treats warnings as errors.  `WALL` is off by
default but is recommended for development and integration builds.

    cmake -S . -B tmp -DWALL=ON

[41]: https://clang.llvm.org/docs/LeakSanitizer.html
[42]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options


## License
`astr` is made available under a BSD-style license; see the [LICENSE][91] file
for details.

[91]: https://git.sr.ht/~donmcc/astr/tree/master/LICENSE
