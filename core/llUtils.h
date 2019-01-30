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
#include "wtermin.h"

NS_LL_BEGIN

class Utils
{
public:
    static void log(const std::string& msg);
    static void assertX(const char* msg, const char* file, int lineno);
};

extern const char* LL_STRING_EMPTY;

#ifdef LL_DEBUG
    #define LLASSERT(x, msg)\
        if (!(x))\
            Utils::assertX(msg, __FILE__, __LINE__)
#else
    #define LLASSERT(x, msg)
#endif

#ifdef LL_DEBUG
#define LLLOG(format, ...) WT_Trace(format, __VA_ARGS__)
#else
#define LLLOG(format)
#endif

NS_LL_END

#endif
