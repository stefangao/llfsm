//============================================================================
// Name        : TestCase.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __TEST_CASE_H__
#define __TEST_CASE_H__

#include "lianli.h"

#define DECLARE_TESTCASE_FACTORY(testCaseClass)\
public:\
static testCaseClass* create()\
{\
    auto testCase = new testCaseClass();\
    if (!testCase->onInit())\
    {\
        delete testCase;\
        testCase = nullptr;\
    }\
    return testCase;\
}

class TestCase
{
public:
    TestCase();
    virtual ~TestCase();

    virtual void start();
    virtual void stop();

protected:
    virtual bool onInit();

    lianli::FSM* mMainFSM;

    DECLARE_TESTCASE_FACTORY(TestCase)
};

#endif
