#include <cstdlib>

#include "application.hpp"

extern "C" {
#include "base/base.h"
}


int
main(int argc, char *argv[])
{
    Application application;
    application.run();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
