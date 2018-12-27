//============================================================================
// Name        : llTimer_cocos2dx.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

NS_LL_BEGIN

int setTimer(int interval, const std::function<void(int tid, const void* userData)>& func, const void* userData)
{
    return 0;
}

bool killTimer(int tid)
{
    return true;
}

void postCallback(const void* userData, const std::function<void(const void* userData)>& func)
{

}

NS_LL_END
