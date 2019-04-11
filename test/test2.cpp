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
    fsm->create("fsm2");
    fsm->start();

    return true;
}

namespace test2 {
/////////// FSMA ///////////
BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, S_ROOT,    SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    0)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)
    TRANS_ENTRY(TEST1, "TestEvt1", TEST2)
END_TRANS_TABLE()

bool FSMA::Test1::onEventProc(const std::string& evtName, EvtData& evtData)
{
    return true;
}

void FSMA::Test1::onHeartBeat()
{
    postEvent("TestEvt1");
}

bool FSMA::Test2::onEventProc(const std::string& evtName, EvtData& evtData)
{
    return true;
}

void FSMA::Test2::onHeartBeat()
{
    me()->stop();
}

}
