API Conventions
===============


Allocation Functions
--------------------

Allocate a struct:

    struct foo *
    foo_alloc(char const *name, int age);

    struct foo *
    foo_alloc_copy(struct foo const *foo);

The caller is responsible for calling the matching `foo_free()` function.  The
function will exit the process if memory cannot be allocated.


Free an allocated struct:

    void
    foo_free(struct foo *foo);

The given pointer may be `NULL`.


Allocate a string or struct value calculated from a struct:

    char *
    foo_alloc_description(struct foo *foo);

    struct bar *
    foo_alloc_bar(struct foo *foo);

The caller is responsible for calling `free_or_die()` (or `bar_free()` if it
exists) on the returned value.  The function will exit the process if memory
cannot be allocated.


Container Functions
-------------------

Add a string or struct to a container:

    void
    foo_add_description(char *description);

    void
    foo_add_bar(struct bar *bar);

The string or struct must be heap allocated.  The container takes ownership of
the given string or struct.  The function will exit the process if memory
cannot be allocated.


Add a copy of a string or struct to a container:

    char *
    foo_copy_description(char const *description);

    struct bar *
    foo_copy_bar(struct bar const *bar);

The container will make a copy of the given string or struct and returns a
pointer to the copy.  The function will exit the process if memory cannot be
allocated.


Remove a string or struct from a container:

    void
    foo_remove_description(char *description);

    void
    foo_remove_bar(struct bar *bar);

The container will relinquish ownership of the given string or struct.  The
caller is now responsible for freeing it.


Container Child Functions
-------------------------

Given a struct that knows its parent container:

    struct bar {
        struct foo *parent;
        // ...
    };


Add a copy of the struct to its parent container:

    struct bar *
    bar_copy(struct bar const *bar);

A pointer to the copy is returned.  The parent container has ownership of the
copy.


Remove the child from its parent container and free it:

    void
    bar_drop(struct bar *bar);

The pointer to the struct is invalid after this call.
