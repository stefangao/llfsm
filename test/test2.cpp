//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test2.h"
using namespace test2;

TestCase2::TestCase2()
{

}

TestCase2::~TestCase2()
{

}

bool TestCase2::onInit()
{
    FSM *fsm = new FSMA();
    fsm->create("TestFsmA");
    fsm->start();

    Utils::log("switch to DAEMON");
    fsm->switchState(FSMA::DAEMON);

    Utils::log("switch to TEST1");
    fsm->switchState(FSMA::TEST1);

    Utils::log("switch to TEST4");
    fsm->switchState(FSMA::TEST4);

    Utils::log("stop...");
    fsm->stop();
    fsm->destroy();
    return true;
}

namespace test2 {
/////////// FSMA ///////////
BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, FSM::ROOT, FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    FSM::SFL_ZERO)
    STATE_ENTRY(TEST3,  Test3,  TEST1,     FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST4,  Test3,  TEST2,     FSM::SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)

END_TRANS_TABLE()

}
