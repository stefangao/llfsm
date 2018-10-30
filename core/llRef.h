//============================================================================
// Name        : llRef.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_REF_H__
#define __LL_REF_H__

#include "llConst.h"

NS_LL_BEGIN

class Ref
{
protected:
    Ref();

public:
    virtual ~Ref(); //necessary although it is empty
    void retain();
    void release();

protected:
    int mRefCount;
};

NS_LL_END

#endif
