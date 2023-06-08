#ifndef FIENDS_PTR_HPP_INCLUDED
#define FIENDS_PTR_HPP_INCLUDED


#include "fiends/fiends.hpp"


template<typename P, typename D>
unique_ptr<P, D>
makeUnique(P *pointer, D deleter)
{
    return unique_ptr<P, D>(pointer, deleter);
}


#endif
