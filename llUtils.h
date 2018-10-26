//============================================================================
// Name        : llUtils.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_UTILS_H__
#define __LL_UTILS_H__

#include "llConst.h"

NS_LL_BEGIN

#ifdef LL_DEBUG
    void __ll_assert(const char* msg, const char* file, int lineno);
    #define LLASSERT(x, msg) \
        if (!(x)) \
            __ll_assert(msg, __FILE__, __LINE__)
#else
    #define LLASSERT(x, msg)
#endif

extern const char* LL_STRING_EMPTY;

class Utils
{
public:
    static void Log(const std::string& msg);
};

NS_LL_END

#endif
