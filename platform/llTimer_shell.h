//============================================================================
// Name        : It is the timer implementation on shell
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include "shell/msgloop.h"
#include "../../test//AppDelegate.h"

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

static AppDelegate* gAppDelegate = nullptr;
//the app entry for shell
int main(int argc, const char * argv[])
{
    lianli::postCallback([](const void* userData) {
        gAppDelegate = new AppDelegate();
    });
    llshell::run_msgloop([](const std::string& cmd, std::vector<std::string>& params)
        {
            if (gAppDelegate)
            {
                if (params.size() >= 1)
                {
                    std::string fsm = cmd;
                    std::string evtName = params[0];
                    std::vector<std::string> evtParams;
                    evtParams.assign(params.begin() + 1, params.end());
                    gAppDelegate->onUserEvent(fsm, evtName, evtParams);
                }
            }
        });

    lianli::Utils::log("main end");
    return 0;
}

