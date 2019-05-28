//============================================================================
// Name        : TestCase.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "TestCase.h"

TestCase::TestCase()
{
    mMainFSM = nullptr;
}

TestCase::~TestCase()
{

}

bool TestCase::onInit()
{
    return true;
}

void TestCase::start()
{
    if (mMainFSM)
    {
        mMainFSM->start();
    }
}

void TestCase::stop()
{
    if (mMainFSM)
    {
        mMainFSM->stop();
    }
}
