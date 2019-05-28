//============================================================================
// Name        : AppDelegate.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_APPDELEGATE_H__
#define __LL_APPDELEGATE_H__

#include "lianli.h"
#include "TestCase.h"
USING_NS_LL;

class AppDelegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void onUserEvent(const std::string& fsmName, const std::string& evtName, lianli::EvtStream& evtData);

    void prompt();

protected:
    std::vector<TestCase*> mTestSuite;
    TestCase* mCurrTestCase;
};

#endif
