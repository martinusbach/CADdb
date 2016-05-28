#include <iostream>

#include "my_logger.h"

int main(int, char*[])
{
    std::cout << "Hello World!" << std::endl;

    log_init();

    LOG(trace)   << "A trace severity message";
    LOG(debug)   << "A debug severity message";
    LOG(info)    << "An informational severity message";
    LOG(warning) << "A warning severity message: " << 1 << ", " << 1.0/3.0;
    LOG(error)   << "An error severity message";
    LOG(fatal)   << "A fatal severity message";

    return 0;
}

