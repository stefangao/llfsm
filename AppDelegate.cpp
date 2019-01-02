//============================================================================
// Name        : AppDelegate.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "AppDelegate.h"
#include "test/test1.h"

AppDelegate::AppDelegate()
{
#if 0
    //test the interfaces: postCallback and setTimer
    Utils::log("gwas: postCallback E1");
    lianli::postCallback(nullptr, [](const void* userData) {
        Utils::log("gwas: postCallback X1");
    });

    Utils::log("gwas: postCallback E2");
    lianli::postCallback(nullptr, [](const void* userData) {
        Utils::log("gwas: postCallback X2");

        setTimer(2000, [](int tid, const void* userData) {
            Utils::log("time out");
        }, nullptr);
    });
#endif

    //run test case1
    TestCase1::create();
}

AppDelegate::~AppDelegate()
{

}

void AppDelegate::onUserEvent(const std::string& fsmName, const std::string& evtName, std::vector<std::string>& evtParams)
{
    auto defaultContext = Context::getDefault();
    auto fsm = defaultContext.find(fsmName);
    if (fsm)
    {
        EvtData evtData;
        for (auto& param : evtParams)
        {
            evtData.write(param);
        }
        fsm->postEvent(evtName, evtData);
    }
}
