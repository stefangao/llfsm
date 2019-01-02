//============================================================================
// Name        : test1.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test1.h"
using namespace test1;

TestCase1::TestCase1()
{

}

TestCase1::~TestCase1()
{

}

bool TestCase1::onInit()
{
    FSM *fsm = new FSMA();
    fsm->create("TestFsmA");
    fsm->start();
    Utils::log("start...");

    EvtData data;
    data.write("%d %d", 123, 78);
    fsm->postEvent("TestEvt1", data);


#if 0
    Utils::log("stop...");
    fsm->stop();
    fsm->destroy();
    Utils::log("destroy end...");
#endif
    return true;
}

namespace test1 {
/////////// FSMA ///////////
BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, S_ROOT,    SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    SFL_ZERO)
    STATE_ENTRY(TEST3,  Test3,  TEST1,     SFL_ACTIVE)
    STATE_ENTRY(TEST4,  Test4,  TEST3,     SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)
    TRANS_ENTRY(TEST1, "TestEvt1", S_NONE, TFL_ZERO)
    TRANS_ENTRY(TEST4, "TestEvt1", TEST2,  TFL_ZERO)
    TRANS_ENTRY(TEST3, "TestEvt1", S_NONE, TFL_ZERO)
END_TRANS_TABLE()

}
