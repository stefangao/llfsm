//============================================================================
// Name        : llTimer_shell.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include "shell/msgloop.h"
#include "../AppDelegate.h"

NS_LL_BEGIN

int setTimer(int interval, const std::function<void(int tid, const void* userData)>& func, const void* userData)
{
    return llshell::setTimer(interval, func, userData, false);
}

bool killTimer(int tid)
{
    return llshell::killTimer(tid);
}

void postCallback(const void* userData, const std::function<void(const void* userData)>& func)
{
    llshell::postCallback(userData, [func](int, const void* userData) {
        func(userData);
    });
}

NS_LL_END

//the app entry for shell
int main(int argc, const char * argv[])
{
    lianli::postCallback(nullptr, [](const void* userData) {
        new AppDelegate();
    });
    llshell::start_msgloop();
    return 0;
}

