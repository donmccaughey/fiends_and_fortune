#include <cstdlib>

#include "fiends/application/application.hpp"

extern "C" {
#include "base/base.h"
}


int
main(int argc, char *argv[])
{
    Application application;
    application.run();
    return EXIT_SUCCESS;
}
