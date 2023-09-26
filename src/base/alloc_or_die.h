/* alloc_or_die. https://github.com/donmccaughey/alloc_or_die
 Copyright (c) 2014, Don McCaughey. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 - Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 - Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef FNF_BASE_ALLOC_OR_DIE_H_INCLUDED
#define FNF_BASE_ALLOC_OR_DIE_H_INCLUDED


#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


////////// Building Blocks //////////

// Prints an error message and exit.  If `errno' is zero, it is set to ENOMEM.
// Prints the error message for `errno' to `stderr', then exits the process
// with `errno' as the status code.
inline void
print_error_and_die(void)
{
    if ( ! errno) errno = ENOMEM;
    perror(NULL);
    exit(errno);
}

// Checks that the `memory' pointer is not null.  If `memory' is not NULL,
// increments `alloc_or_die_count' and returns `memory'.  If `memory' is NULL,
// sets `errno' to ENOMEM if `errno' is zero, then prints the error message for
// `errno' to `stderr' and exits the process with `errno' as the status code.
inline void *
not_null_or_die(void *memory)
{
    if ( ! memory) print_error_and_die();
    return memory;
}

// Calculates the size of an array, checking for overflow.  Returns the size if
// it is less than or equal to SIZE_MAX.  If the calculated size exceeds
// SIZE_MAX, sets `errno' to EOVERFLOW, prints the error message for `errno' to
// `stderr' and exits the process with `errno' as the status code.
size_t
array_size_or_die(size_t count, size_t element_size);


////////// Core Allocation Wrappers //////////

// Wrapper for calloc().  Increments `alloc_or_die_count' on success.  On
// failure, prints the error message for `errno' to `stderr' and exits the
// process with `errno' as the status code.
inline void *
calloc_or_die(size_t count, size_t element_size)
{
    return not_null_or_die(calloc(count, element_size));
}


////////// Formatting Functions //////////

// Allocates a formatted string.  On success, sets `*string' contains a pointer
// to the newly allocated, formatted string, increments `alloc_or_die_count',
// and returns the number of characters in the formatted string (excluding the
// terminating zero).  On failure, prints the error message for `errno' to
// `stderr' and exits the process with `errno' as the status code.
inline int
vasprintf_or_die(char **string, const char *format, va_list arguments)
{
    int result = vasprintf(string, format, arguments);
    if (-1 == result) print_error_and_die();
    return result;
}


#endif
