//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "TestCase.h"
USING_NS_LL;

class TestCase5 : public TestCase
{
protected:
    virtual bool onInit();

    DECLARE_TESTCASE_FACTORY(TestCase5)
};
