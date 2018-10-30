//============================================================================
// Name        : llEvtData.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_EVTDATA_H__
#define __LL_EVTDATA_H__

#include "llConst.h"

NS_LL_BEGIN

class EvtData
{
public:
    EvtData(int bufLen = 0)
    {
    }

protected:
    virtual ~EvtData()
    {
    }

public:
    static const EvtData EMPTY;
};

NS_LL_END

#endif
