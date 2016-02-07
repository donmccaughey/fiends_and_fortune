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

Containers should manage creating and destroying child strings and structs.


Create a new child struct to a container:

    struct bar *
    foo_add_bar(struct foo *foo, char const *color, int weight);

The foo struct owns and manages the memory for the returned struct bar *.


Copy an existing child struct into a container:

    struct bar *
    foo_copy_bar(struct foo *foo, struct bar *bar);


Destroy a child struct in a container:

    void
    foo_delete_bar(struct foo *foo, struct bar *bar);

The struct bar * is invalid after calling this function.
