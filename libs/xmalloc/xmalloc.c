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
#include "xmalloc.h"

#include <stdint.h>
#include <string.h>


size_t
arraysize(size_t count, size_t element_size, bool *overflow)
{
    static size_t const sqrt_size_max = (size_t)1 << (sizeof(size_t) * 4);
    if (count >= sqrt_size_max || element_size >= sqrt_size_max) {
        if (element_size && count > SIZE_MAX / element_size) {
            if (overflow) *overflow = true;
            return SIZE_MAX;
        }
    }
    if (overflow) *overflow = false;
    return count * element_size;
}


#ifdef XMALLOC_NEEDS_REALLOCARRAY
#include <errno.h>

void *
reallocarray(void *memory, size_t count, size_t element_size)
{
    bool overflow;
    size_t size = arraysize(count, element_size, &overflow);
    if (overflow) {
        errno = ENOMEM;
        return NULL;
    } else {
        return realloc(memory, size);
    }
}

#endif


#ifdef XMALLOC_NEEDS_RECALLOCARRAY

void *
recallocarray(void *memory, size_t old_count, size_t new_count, size_t element_size)
{
    void *new_memory = reallocarray(memory, new_count, element_size);
    if (new_memory && (new_count > old_count)) {
        void *start = new_memory + (old_count * element_size);
        size_t count = (new_count - old_count) * element_size;
        memset(start, 0, count);
    }
    return new_memory;
}

#endif


void *
xcalloc(size_t count, size_t element_size)
{
    void *new_memory;
    if (count && element_size) {
        new_memory = calloc(count, element_size);
    } else {
        new_memory = malloc(1);
    }
    if (!new_memory) abort();
    return new_memory;
}


void *
xmalloc(size_t size)
{
    void *new_memory;
    if (size) {
        new_memory = malloc(size);
    } else {
        new_memory = malloc(1);
    }
    if (!new_memory) abort();
    return new_memory;
}


void *
xrealloc(void *memory, size_t size)
{
    void *new_memory;
    if (size) {
        new_memory = realloc(memory, size);
    } else {
        free(memory);
        new_memory = malloc(1);
    }
    if (!new_memory) abort();
    return new_memory;
}


void *
xreallocarray(void *memory, size_t count, size_t element_size)
{
    void *new_memory;
    if (count && element_size) {
        new_memory = reallocarray(memory, count, element_size);
    } else {
        free(memory);
        new_memory = malloc(1);
    }
    if (!new_memory) abort();
    return new_memory;
}


void *
xrecallocarray(void *memory, size_t old_count, size_t new_count, size_t element_size)
{
    void *new_memory;
    if (new_count && element_size) {
        new_memory = recallocarray(memory, old_count, new_count, element_size);
    } else {
        free(memory);
        new_memory = malloc(1);
    }
    if (!new_memory) abort();
    return new_memory;
}
