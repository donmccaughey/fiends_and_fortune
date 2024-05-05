# xmalloc

A small C 99 library for fail-fast memory allocation functions.

[![builds.sr.ht status](https://builds.sr.ht/~donmcc/xmalloc.svg)](https://builds.sr.ht/~donmcc/xmalloc?)


## Functions

Functions with the prefix `x` will always return a valid pointer or abort the
process.  There is never a need to check these functions for failure.

Functions without the `x` prefix may return `NULL` on failure.  These include
the `reallocarray()` and `recallocarray()` polyfills and the `arraydup()` and
`memdup()` convenience functions.

Use `free()` on all pointers returned by xmalloc.

You can add xmalloc files directly to your project if you set the necessary
[preprocessor definitions][21] or link against the static library produced by
the included CMake build system.

[21]: https://git.sr.ht/~donmcc/xmalloc#preprocessor-definitions

### § `xmalloc.h` and `xmalloc.c`

This module wraps the core memory allocation functions defined in `stdlib.h`. 
It stands alone and only requires the C standard library.

    void *
    xcalloc(size_t count, size_t element_size)
    
    void *
    xmalloc(size_t size);
    
    void *
    xrealloc(void *memory, size_t size)
    
    void *
    xreallocarray(void *memory, size_t count, size_t element_size);
    
    void *
    xrecallocarray(void *memory, size_t old_count, size_t new_count, size_t element_size);

These functions are fail-fast wrappers for `malloc()`, `calloc()`, `realloc()`,
`reallocarray()` and `recallocarray()`. They have the same parameters as the
functions they wrap. Note that these wrappers will always return a non-`NULL`
pointer even on systems where `malloc()`, `calloc()`, `realloc()`,
`reallocarray()` or `recallocarray()` return `NULL` for zero-sized allocations.

    size_t
    arraysize(size_t count, size_t element_size, bool *overflow)

The `arraysize()` function calculates the size in bytes from a `count` of
elements and the `element_size` in bytes.  If overflow is detected, `SIZE_MAX`
is returned and `*overflow` will contain `true`.  Alternately you can pass in
`NULL` for `overflow` and check the return value against `SIZE_MAX` if your
program would never allocate an array that large.

    void *
    reallocarray(void *memory, size_t count, size_t element_size);

    void *
    recallocarray(void *memory, size_t old_count, size_t new_count, size_t element_size);

If your system does not include `reallocarray()` or `recallocarray()`, you can
define `XMALLOC_NEEDS_REALLOCARRAY` and `XMALLOC_NEEDS_RECALLOCARRAY` to include
these polyfills.

### § `xstring.h` and `xstring.c`

This module wraps allocation functions defined in `string.h`.  It depends on
`xmalloc.h` and `xmalloc.c`.

    void *
    arraydup(void const *array, size_t count, size_t element_size);
    
    void *
    memdup(void const *memory, size_t size);

Non-standard companions to `strdup()`.  They will return `NULL` on memory
allocation failure, but have fail-fast wrappers below.  The `array` and `memory`
parameters may not be `NULL`; in debug builds this will cause an assertion
failure; if `NDEBUG` is defined then `NULL` is returned and `errno` is set to
`EINVAL`.
    
    void *
    xarraydup(void const *array, size_t count, size_t element_size);
    
    void *
    xmemdup(void const *memory, size_t size);
    
    char *
    xstrdup(char const *string);
    
    char *
    xstrndup(char const *string, size_t count);

These functions are fail-fast wrappers for `arraydup()`, `memdup()`, `strdup()`
and `strndup()`.  If the `array`, `memory` or `string` parameters are `NULL`, a
zero-sized allocation or empty string is returned.  

### § `xstdio.h` and `xstdio.c`

This module wraps allocation functions defined in `stdio.h`.  It depends on
`xmalloc.h` and `xmalloc.c`.

    int
    xasprintf(char **s, char const *format, ...);
    
    int
    xvasprintf(char **s, char const *format, va_list arguments);

These functions are fail-fast wrappers for `asprintf()` and `vasprintf()`.  The
`s` and `format` parameters may not be `NULL`; in debug builds this will cause
an assertion failure; if `NDEBUG` is defined then `abort()` will be called.

### § `xlibgen.h` and `xlibgen.c`

This module wraps allocation functions defined in `libgen.h`.  It depends on
`xmalloc.h` and `xmalloc.c`.

    char *
    xbasename(char const *path);
    
    char *
    xdirname(char const *path);

The POSIX [`basename()`][41] and [`dirname()`][42] functions take non-`const`
path strings which may be modified.  They may return a pointer to internal
storage which can be overwritten by subsequent calls.  To use these functions
safely, the caller needs to defensively copy both the path argument and the
returned value.  The `xbasename()` and `xdirname()` wrappers do this for you. 
The path parameter is `const` and the return value is always a newly allocated
string.

[41]: https://man.bsd.lv/POSIX-2013/basename.3
[42]: https://man.bsd.lv/POSIX-2013/dirname.3

### § `xunistd.h` and `xunistd.c`

This module wraps allocation functions defined in `unistd.h`.

    char *
    xgetcwd(void);

The `xgetcwd()` function wraps `getcwd()` and relies on a common POSIX extension
where `getcwd()` will allocate a buffer for the returned path if given a `NULL`
buffer argument.


## Building

The xmalloc code is small--you can drop the source files you need into your
project if that suits you.  On all systems other than OpenBSD, you must define
`XMALLOC_NEEDS_RECALLOCARRAY`.  On macOS, you must define
`XMALLOC_NEEDS_REALLOCARRAY`.  On Linux, you must define `_GNU_SOURCE` to use
`xstdio.h` and `xstdio.c`.

### CMake Build System

Alternately, you can use [CMake][61] to build xmalloc as a static library. CMake
will detect your system and compiler and automatically set all the necessary
preprocessor definitions.  To build, test and install xmalloc with CMake:

    git clone https://git.sr.ht/~donmcc/xmalloc
    cd xmalloc
    cmake -S . -B tmp
    cmake --build tmp --target all test
    cmake --install tmp

### CMake Build Options

To build with the [AddressSanitizer][62] enabled, set the `ADDRESS_SANITIZER`
option to `ON`.

    cmake -S . -B tmp -DADDRESS_SANITIZER=ON

Setting the `WALL` option to `ON` turns on [additional warnings][63] using the 
`-Wall` compiler flag and also treats warnings as errors.  `WALL` is off by
default but is recommended for development and integration builds.

    cmake -S . -B tmp -DWALL=ON

To disable test targets, set the `BUILD_TESTING` option to `OFF`.

    cmake -S . -B tmp -DBUILD_TESTING=OFF

[61]: https://cmake.org
[62]: https://clang.llvm.org/docs/LeakSanitizer.html
[63]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options


## Preprocessor Definitions

These macros control optional features of xmalloc.  They are configured
automatically if you use the CMake build system.

### Polyfills

- `XMALLOC_NEEDS_REALLOCARRAY` - define to include a [`reallocarray()`][51]
    polyfill (needed by macOS)

- `XMALLOC_NEEDS_RECALLOCARRAY` - define to include a [`recallocarray()`][52]
    polyfill (needed by all systems other than OpenBSD)

[51]: https://man.bsd.lv/reallocarray
[52]: https://man.bsd.lv/recallocarray

### GNU Source

- `_GNU_SOURCE` - needed on Linux to make [`vasprintf()`][53] available

[53]: https://man.bsd.lv/Linux-5.13/vasprintf


## Prior Art

There are a number of other libraries that provide versions of `xmalloc()`:

- GNU [libiberty][71] ([code][72])
- Debian [publib][73] ([code][74]) ([man page][75])
- [rra-c-util library][76] ([code][77])
- Example `xmalloc()` function from the [GNU C Library Reference Manual][78]

[71]: https://gcc.gnu.org/onlinedocs/libiberty/Memory-Allocation.html
[72]: https://codebrowser.dev/gcc/libiberty/xmalloc.c.html
[73]: https://tracker.debian.org/pkg/publib
[74]: https://salsa.debian.org/debian/publib/-/blob/master/alloc/xmalloc.c
[75]: https://manpages.debian.org/bookworm/publib-dev/xmalloc.3pub.en.html
[76]: https://www.eyrie.org/~eagle/software/rra-c-util/xmalloc.html
[77]: https://github.com/rra/rra-c-util/blob/main/util/xmalloc.c
[78]: https://www.gnu.org/software/libc/manual/html_node/Malloc-Examples.html

Some programs add their own `xmalloc()` functions:

- GNU [Inetutils][81] ([code][82])

[81]: https://www.gnu.org/software/inetutils/
[82]: https://www.gnu.org/software/inetutils/coverage/lib/xalloc.h.gcov.html

### Peer Languages

The C++ `new` operator throws a [`bad_alloc` exception][21] if a memory
allocation fails.  Similarly, the [Rust global allocator][22] will [panic by
default][23].  While it's possible to catch and handle this exception or panic
locally, doing so has the same set of drawbacks that `NULL` checks have in C. If
code in either language handles out of memory conditions at all, it is typically
done around a high level unit of work like a task or request handler.

[21]: https://en.cppreference.com/w/cpp/memory/new/operator_new#Exceptions
[22]: https://doc.rust-lang.org/alloc/alloc/trait.GlobalAlloc.html#errors
[23]: https://doc.rust-lang.org/alloc/alloc/fn.handle_alloc_error.html


## Why Fail-Fast?

The C language was born in an era when RAM was measured in kilobytes.  Data
often exceeded the amount of free RAM in a system, so programmers carefully
checked each memory allocation for failure.  This low level control is why C is
still the language of choice for small embedded systems.

Modern systems measure RAM in gigabytes and have virtual memory managers that
seamlessly swap chunks of memory between RAM and storage.  Routine small memory
allocations rarely fail--there is almost always some logical address space
available to a process that can be mapped to physical memory, and there's almost
always some chunk of physical memory that can be swapped out to disk. When the
machine hits its absolute limits and there's no RAM or swap space left, the
operating system may choose to [kill a process][11] rather than fail a memory
allocation request.

[11]: https://www.kernel.org/doc/gorman/html/understand/understand016.html

Diligently checking the returned value of each `malloc()` call creates a
significant overhead in C code:

- it takes more time to read and write the error checking code
- it is _very_ difficult to effectively test memory allocation failures when 
    initializing complex graphs of allocated structures
- there are limited options available when memory is exhausted; even printing or
    logging an error message may fail

Use of dynamically allocated memory tends to fall into two categories: lots of
smallish "routine" allocations (formatting messages, building object graphs,
etc.) and a few (sometimes zero) "non-routine" allocations for core operations
(image processing, matrix math, etc.)

On a modern computer system with virtual memory management, the overhead of this
diligent correctness provides almost no benefit for routine allocations--the
process is likely to be killed before it has to handle a routine `malloc()`
failure.

_Usually the best thing to do when a routine memory allocation fails is to halt
the process._


## License

The xmalloc library is made available under a BSD-style license.  See the 
[`LICENSE`][91] file for details.

[91]: https://git.sr.ht/~donmcc/xmalloc/tree/main/LICENSE
