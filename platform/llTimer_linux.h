//============================================================================
// Name        : It is the timer implementation on linux
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include "shell/msgloop.h"

NS_LL_BEGIN

int setTimer(int interval, const std::function<void(int tid, const void* userData)>& func, const void* userData)
{
    return llshell::setTimer(interval, func, userData, false);
}

bool killTimer(int tid)
{
    return llshell::killTimer(tid);
}

void postCallback(const std::function<void(const void* userData)>& func, const void* userData)
{
    llshell::postCallback(userData, [func](int, const void* userData) {
        func(userData);
    });
}

NS_LL_END
