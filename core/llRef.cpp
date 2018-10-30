//============================================================================
// Name        : llRef.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "llRef.h"
#include "llUtils.h"

NS_LL_BEGIN

Ref::Ref()
    : mRefCount(1)
{

}

Ref::~Ref()
{

}

void Ref::retain()
{
    LLASSERT(mRefCount > 0, "error: ref count must be > 0");
    mRefCount++;
}

void Ref::release()
{
    LLASSERT(mRefCount > 0, "error: ref count must be > 0");
    mRefCount--;

    if (mRefCount == 0)
    {
        delete this;
    }
}

NS_LL_END
