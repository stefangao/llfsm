//============================================================================
// Name        : main entry of linux app
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include "lianli.h"
#include "platform/generic/msgloop.h"
#include "../AppDelegate.h"

static AppDelegate* gAppDelegate = nullptr;
int main(int argc, const char * argv[])
{
    lianli::postCallback([](const void* userData) {
        gAppDelegate = new AppDelegate();
    });
    llgeneric::run_msgloop([](const std::string& cmd, std::vector<std::string>& params)
        {
            if (gAppDelegate)
            {
                if (params.size() >= 1)
                {
                    std::string fsm = cmd;
                    std::string evtName = params[0];

                    lianli::EvtStream evtData;
                    for (auto iter = params.begin() + 1; iter != params.end(); iter++)
                    {
                        evtData << *iter;
                    }
                    gAppDelegate->onUserEvent(fsm, evtName, evtData);
                }
                else
                {
                    gAppDelegate->onUserEvent(cmd, LL_STR_EMPTY, (lianli::EvtStream&)lianli::EvtStream::EMPTY);
                }
            }
        });

    LLLOG("main end");
    return 0;
}

