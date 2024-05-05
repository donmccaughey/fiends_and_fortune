# TODO

- man page
- doxygen docs
- CMake debug vs release vs "default" builds
- add CMake `FindXmalloc.cmake` module
- `xmalloc` functions
  - `aligned_alloc()`
  - `valloc()`
- `xstdio` functions
  - `xgetdelim()`
  - `xgetline()`
- `xinet` functions
  - `inet_ntoa()` / `inet_ntoa_r()` 
  - `xinet_ntop()`
- consider supporting
  - `memalign()` and `posix_memalign()`
  - `pvalloc()` (GNU extension)
  - should there be a wrapper for `mmap()`?
