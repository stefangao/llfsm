//============================================================================
// Name        : llUtils.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_UTILS_H__
#define __LL_UTILS_H__

#include <string>
#include "llConst.h"

NS_LL_BEGIN

class Utils
{
public:
    static void __log(const char* format,...);
    static void __assert(const char* file, int lineno, const char* format,...);
};

extern const char* LL_STRING_EMPTY;

#ifdef LL_DEBUG
    #define LLASSERT(x, format, ...)\
        if (!(x))\
            Utils::__assert(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define LLASSERT(x, msg)
#endif

#ifdef LL_DEBUG
#define LLLOG(format,...) Utils::__log(format, ##__VA_ARGS__)
#else
#define LLLOG(format)
#endif

NS_LL_END

#endif
