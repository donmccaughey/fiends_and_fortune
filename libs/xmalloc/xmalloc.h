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
#ifndef XMALLOC_H_INCLUDED
#define XMALLOC_H_INCLUDED


#include <stdbool.h>
#include <stdlib.h>


size_t
arraysize(size_t count, size_t element_size, bool *overflow);

#ifdef XMALLOC_NEEDS_REALLOCARRAY
void *
reallocarray(void *memory, size_t count, size_t element_size);
#endif

#ifdef XMALLOC_NEEDS_RECALLOCARRAY
void *
recallocarray(void *memory, size_t old_count, size_t new_count, size_t element_size);
#endif

void *
xcalloc(size_t count, size_t element_size);

void *
xmalloc(size_t size);

void *
xrealloc(void *memory, size_t size);

void *
xreallocarray(void *memory, size_t count, size_t element_size);

void *
xrecallocarray(void *memory, size_t old_count, size_t new_count, size_t element_size);


#endif
