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
    static void _ll_log(const char* format,...);
    static void _ll_assert(const char* file, int lineno, const char* format,...);
};

extern const char* LL_STR_EMPTY;

#ifdef LL_DEBUG
    #define LLASSERT(x, format, ...)\
        if (!(x))\
            Utils::_ll_assert(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define LLASSERT(x, msg)
#endif

#ifdef LL_DEBUG
#define LLLOG(format,...) lianli::Utils::_ll_log(format, ##__VA_ARGS__)
#else
#define LLLOG(format)
#endif

NS_LL_END

#endif
