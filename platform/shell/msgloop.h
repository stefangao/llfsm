//============================================================================
// Name        : msgloop.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_MSGLOOP_H__
#define __LL_MSGLOOP_H__

#include <functional>
#include <vector>
#include <string>

namespace llshell {

typedef std::function<void(int id, const void*)> MsgCallbackFunc;
typedef std::function<void(const std::string& cmd, std::vector<std::string>& params)> CmdCallbackFunc;

int setTimer(int interval, const MsgCallbackFunc& func, const void* userData, bool once);
bool killTimer(int tid);
void postCallback(const void* userData, const MsgCallbackFunc& func);

int run_msgloop(const CmdCallbackFunc& cmdCallbackFun = nullptr);

}

#endif
