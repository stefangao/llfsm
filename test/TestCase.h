//============================================================================
// Name        : TestCase.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __TEST_CASE_H__
#define __TEST_CASE_H__

#include "../core/lianli.h"

#define DECLARE_TESTCASE_FACTORY(testCaseClass)\
public:\
static testCaseClass* create()\
{\
    auto testCase = new testCaseClass();\
    if (!testCase->onInit());\
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

protected:
    virtual ~TestCase();
    virtual bool onInit();

    DECLARE_TESTCASE_FACTORY(TestCase)
};

#endif
