//============================================================================
// Name        : main entry of linux app
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include "lianli.h"
#include "platform/shell/msgloop.h"
#include "../AppDelegate.h"

static AppDelegate* gAppDelegate = nullptr;
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

    LLLOG("main end");
    return 0;
}

