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

int  setTimer(int interval, std::function<void(void*)> func, bool once = false);
bool killTimer(int tid);
void postCallback(std::function<void(void*)> func);

NS_LL_END

#endif
