#ifndef FNF_BASE_INT_H_INCLUDED
#define FNF_BASE_INT_H_INCLUDED


#include <base/rnd.h>


inline void
fill(int *values, int count)
{
    for (int i = 0; i < count; ++i) values[i] = i;
}

inline void
fill_shuffled(struct rnd *rnd, int *values, int count)
{
    fill(values, count);
    rnd_shuffle(rnd, values, count, sizeof(int));
}

inline int
max(int i, int j)
{
    return i > j ? i : j;
}

inline int
min(int i, int j)
{
    return i < j ? i : j;
}

inline void
swap(int *i, int *j)
{
    int temp = *i;
    *i = *j;
    *j = temp;
}


#endif
