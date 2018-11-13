//============================================================================
// Name        : llTimer_win.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

NS_LL_BEGIN

int setTimer(int interval, std::function<void(void*)> func, bool once)
{

    return 0;
}

bool killTimer(int tid)
{

    return true;
}

void postCallback(std::function<void(void*)> func)
{

}

NS_LL_END
