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
