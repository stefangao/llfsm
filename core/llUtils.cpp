//============================================================================
// Name        : llUtils.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include "llUtils.h"

#ifdef WIN32
#include <windows.h>
#endif

NS_LL_BEGIN

const char* LL_STR_EMPTY = "";

void Utils::_ll_log(const char *format,...)
{
    char strBuffer[4096] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, format);
    vsnprintf(strBuffer, sizeof(strBuffer) - 1, format, vlArgs);
    va_end(vlArgs);

#ifdef WIN32
    OutputDebugString(strBuffer);
#else
    std::cout << strBuffer << std::endl;
#endif
}

void Utils::_ll_assert(const char* file, int lineno, const char* format,...)
{
    char strBuffer[4096] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, format);
    vsnprintf(strBuffer, sizeof(strBuffer) - 1, format, vlArgs);
    va_end(vlArgs);

    LLLOG("Assert failed: %s\n[%s:%d]\n", strBuffer, file, lineno);
    abort();
}

NS_LL_END
