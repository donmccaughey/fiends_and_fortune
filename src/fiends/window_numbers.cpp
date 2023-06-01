#include "window_numbers.hpp"

#include "fiends.hpp"


WindowNumbers windowNumbers = WindowNumbers();


void
WindowNumbers::release(short number)
{
    inUse[number] = false;
}


short
WindowNumbers::takeNext()
{
    for (short i = 1; i < 10; ++i) {
        if (!inUse[i]) {
            inUse[i] = true;
            return i;
        }
    }
    return wnNoNumber;
}
