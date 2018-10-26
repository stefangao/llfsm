//============================================================================
// Name        : llContext.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_CONTEXT_H__
#define __LL_CONTEXT_H__

#include <string>
#include "llConst.h"

NS_LL_BEGIN

class Context
{
public:
    Context(const std::string& name);

protected:
    std::string mName;

public:
    static const Context DEFAULT;
};

NS_LL_END

#endif
