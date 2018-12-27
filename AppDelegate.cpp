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
    TestCase1::create();

    setTimer(2000, [](int tid, const void* userData) {
        Utils::log("time out");
    }, nullptr);

    /*
    lianli::postCallback(nullptr, [](const void* userData) {
        Utils::log("gwas: postCallback");
    });*/
}

AppDelegate::~AppDelegate()
{

}
