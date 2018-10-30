//============================================================================
// Name        : llUtils.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <stdlib.h>
#include "llUtils.h"

NS_LL_BEGIN

const char* LL_STRING_EMPTY = "";

void Utils::log(const std::string& msg)
{
    std::cout << msg << std::endl;
}

void Utils::assert(const char* msg, const char* file, int lineno)
{
    std::cout << "Assert failed {" << msg << "} " << file << "(" << lineno << ")" << std::endl;
    abort();
}

NS_LL_END
