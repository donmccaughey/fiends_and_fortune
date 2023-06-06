#ifndef FIENDS_PTR_HPP_INCLUDED
#define FIENDS_PTR_HPP_INCLUDED


#include "fiends.hpp"


template<typename T, typename D>
unique_ptr<T, D>
makeUnique(T *p, D deleter)
{
    return unique_ptr<T, D>(p, deleter);
}


#endif
