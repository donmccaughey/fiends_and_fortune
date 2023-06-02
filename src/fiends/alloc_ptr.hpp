#ifndef FIENDS_ALLOC_PTR_HPP_INCLUDED
#define FIENDS_ALLOC_PTR_HPP_INCLUDED


#include "fiends.hpp"


extern "C" void
free_or_die(void *memory);


template<typename T>
using AllocPtr = unique_ptr<T, decltype(&free_or_die)>;


template<typename T>
AllocPtr<T>
makeAllocPtr(T *p)
{
    return AllocPtr<T>(p, &free_or_die);
}


#endif
