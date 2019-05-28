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

    prompt();
}

void AppDelegate::prompt()
{
    LLLOG("\nPlease type the number of test case to run:\n");
    for (int i = 0; i < mTestSuite.size(); i++)
    {
        LLLOG("[%s]: %d\n", mTestSuite[i]->mTitle.c_str(), i + 1);
    }
    LLLOG("(type 0 to stop the running test case)\n");
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

bool isNumber(const std::string& input)
{
    for (int i = 0; i < input.size(); i++)
    {
        char c = input.at(i);
        if (!isdigit(c))
            return false;
    }
    return true;
}

void AppDelegate::onUserEvent(const std::string& fsmName, const std::string& evtName, lianli::EvtStream& evtData)
{
    bool isTestCaseStarted = false;
    auto defaultContext = Context::getDefault();
    if (!evtName.empty())
    {
        auto fsm = defaultContext.find(fsmName);
        if (fsm)
        {
            fsm->postEvent(evtName, evtData);
        }
    }
    else if (isNumber(fsmName))
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
                isTestCaseStarted = true;
            }
        }
    }

    if (!isTestCaseStarted)
        prompt();
}
