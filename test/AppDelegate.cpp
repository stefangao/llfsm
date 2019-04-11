//============================================================================
// Name        : AppDelegate.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include <iostream>
#include "AppDelegate.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"

AppDelegate::AppDelegate()
{
    //run test cases
    //TestCase1::create();
    //TestCase2::create();
    TestCase3::create();
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
            evtData.DataBuf::write(param);
        }
        fsm->postEvent(evtName, evtData);
    }
}
