#ifndef __LLEVTDATA_H__
#define __LLEVTDATA_H__

#include "llConst.h"

NS_LL_BEGIN

class EvtData
{
public:
	EvtData(int bufLen = 0) {}

protected:
	virtual ~EvtData() {}

public:
	static const EvtData EMPTY;
};

NS_LL_END

#endif
