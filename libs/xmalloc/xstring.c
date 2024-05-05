// xmalloc - https://git.sr.ht/~donmcc/xmalloc
// Copyright (c) 2023, Don McCaughey. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "xstring.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


void *
arraydup(void const *array, size_t count, size_t element_size)
{
    bool overflow;
    size_t size = arraysize(count, element_size, &overflow);
    if (overflow) {
        errno = ENOMEM;
        return NULL;
    } else {
        return memdup(array, size);
    }
}


void *
memdup(void const *memory, size_t size)
{
    assert(memory);

    if (!memory) {
        errno = EINVAL;
        return NULL;
    }

    void *copy = malloc(size);
    if (size && copy) memcpy(copy, memory, size);
    return copy;
}


void *
xarraydup(void const *array, size_t count, size_t element_size)
{
    void *copy;
    if (array && count && element_size) {
        copy = arraydup(array, count, element_size);
    } else if (count && element_size) {
        copy = calloc(count, element_size);
    } else {
        copy = malloc(1);
    }
    if (!copy) abort();
    return copy;
}


void *
xmemdup(void const *memory, size_t size)
{
    void *copy;
    if (memory && size) {
        copy = memdup(memory, size);
    } else if (size) {
        copy = calloc(size, 1);
    } else {
        copy = malloc(1);
    }
    if (!copy) abort();
    return copy;
}


char *
xstrdup(char const *string)
{
    char *copy = strdup(string ? string : "");
    if (!copy) abort();
    return copy;
}


char *
xstrndup(char const *string, size_t count)
{
    char *copy = strndup(string ? string : "", count);
    if (!copy) abort();
    return copy;
}
