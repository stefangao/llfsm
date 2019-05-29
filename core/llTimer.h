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

#define LL_PLATFORM_LINUX    1
#define LL_PLATFORM_WIN      0
#define LL_PLATFORM_MAC      0
#define LL_PLATFORM_COCOS2DX 0

NS_LL_BEGIN

int  setTimer(int interval, const std::function<void(int tid, const void* userData)>& func, const void* userData = nullptr);
bool killTimer(int tid);
void postCallback(const std::function<void(const void* userData)>& func, const void* userData = nullptr);

NS_LL_END

#endif
