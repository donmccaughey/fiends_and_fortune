#include <assert.h>
#include <base/base.h>


static void
xcalloc_test(void)
{
    int *memory = xcalloc(100, sizeof(int));
    assert(memory);
    assert(0 == memory[0]);
    free(memory);
}


static void
xmalloc_function_test(void)
{
    int *memory = xmalloc(100 * sizeof(int));
    assert(memory);
    free(memory);
}


static void
xrealloc_test(void)
{
    int *memory = xmalloc(10 * sizeof(int));
    for (int i = 0; i < 10; ++i) {
        memory[i] = i + 1;
    }
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xrealloc(memory, 20 * sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xrealloc(memory, 5 * sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(5 == memory[4]);

    free(memory);

    memory = xrealloc(NULL, 10 * sizeof(int));
    assert(memory);
    free(memory);
}


void
xmalloc_test(void)
{
    xcalloc_test();
    xmalloc_function_test();
    xrealloc_test();
}
