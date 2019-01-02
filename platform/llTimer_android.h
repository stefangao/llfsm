//============================================================================
// Name        : It is the timer implementation on android platform
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

void postCallback(const std::function<void(const void* userData)>& func, const void* userData)
{

}

NS_LL_END
