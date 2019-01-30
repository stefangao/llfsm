//============================================================================
// Name        : It is the timer implementation on windows platform
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <windows.h>
#include <map>

NS_LL_BEGIN

static std::map<int, std::function<void(int tid, const void* userData)>> gTimerMap;

static void CALLBACK TimeOutProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	auto iter = gTimerMap.find(idEvent);
	if (iter != gTimerMap.end())
	{
		auto& func = iter->second;
		func(idEvent, 0);
	}
}

int setTimer(int interval, const std::function<void(int tid, const void* userData)>& func, const void* userData)
{
	int nIDEvent = SetTimer(NULL, 0, interval, TimeOutProc);
	if (nIDEvent > 0)
	{
		gTimerMap.insert(make_pair(nIDEvent, func));
		return nIDEvent;
	}
    return 0;
}

bool killTimer(int tid)
{
	KillTimer(0, tid);
	auto iter = gTimerMap.find(tid);
	if (iter != gTimerMap.end())
	{
		gTimerMap.erase(iter);
	}
    return true;
}

void postCallback(const std::function<void(const void* userData)>& func, const void* userData)
{
	int tid = setTimer(0, [func, userData](int tid, const void*) {
		func(userData);
		killTimer(tid);
	}, 0);
}

NS_LL_END
