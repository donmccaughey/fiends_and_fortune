#ifndef FIENDS_UTIL_WINDOW_NUMBERS_HPP_INCLUDED
#define FIENDS_UTIL_WINDOW_NUMBERS_HPP_INCLUDED


class WindowNumbers {
    bool inUse[10]{};

public:
    void release(short number);
    short takeNext();
};


extern WindowNumbers windowNumbers;


#endif
