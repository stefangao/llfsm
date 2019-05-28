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
#include "test4.h"
#include "test5.h"

AppDelegate::AppDelegate()
{
    mCurrTestCase = nullptr;
    //create test cases
    mTestSuite.push_back(TestCase1::create());
    mTestSuite.push_back(TestCase2::create());
    mTestSuite.push_back(TestCase3::create());
    mTestSuite.push_back(TestCase4::create());
    mTestSuite.push_back(TestCase5::create());
}

AppDelegate::~AppDelegate()
{
    for (auto iter = mTestSuite.begin(); iter != mTestSuite.end(); iter++)
    {
        auto testCase = *iter;
        delete testCase;
    }
    mTestSuite.clear();
}

void AppDelegate::onUserEvent(const std::string& fsmName, const std::string& evtName, lianli::EvtStream& evtData)
{
    auto defaultContext = Context::getDefault();
    if (!evtName.empty())
    {
        auto fsm = defaultContext.find(fsmName);
        if (fsm)
        {
            fsm->postEvent(evtName, evtData);
        }
    }
    else
    {
        int index = atoi(fsmName.c_str());
        if (index >= 0 && index <= mTestSuite.size())
        {
            if (mCurrTestCase)
            {
                mCurrTestCase->stop();
                mCurrTestCase = nullptr;
            }

            if (index > 0)
            {
                auto testCase = mTestSuite[index - 1];
                testCase->start();
                mCurrTestCase = testCase;
            }
        }
    }
}
