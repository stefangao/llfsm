//============================================================================
// Name        : llRef.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_TIMER_H__
#define __LL_TIMER_H__

#include "llConst.h"
#include <functional>

NS_LL_BEGIN

int  setTimer(int interval, const std::function<void(int tid, const void* userData)>& func, const void* userData);
bool killTimer(int tid);
void postCallback(const void* userData, const std::function<void(const void* userData)>& func);

NS_LL_END

#endif
