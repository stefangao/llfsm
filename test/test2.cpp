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
    FSM* fsm = LL_CREATE_FSM(FSMA, "fsm2", Context::DEFAULT);
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
    TRANS_ENTRY(TEST1, "TestEvt1", S_NONE)
END_TRANS_TABLE()

bool FSMA::Test1::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    int a, b;
    evtData >> a >> b;
    LLLOG("FSMA::Test1::onEventProc: %d, %d\n", a, b);
    return true;
}

bool FSMA::Test1::onRequestProc(const std::string& evtName, EvtStream& evtData, EvtStream& rspData)
{
    int a, b;
    evtData >> a >> b;
    LLLOG("FSMA::Test1::onRequestProc: %d, %d\n", a, b);

    rspData << 67 << 89;
    return true;
}


void FSMA::Test1::onHeartBeat()
{
    EvtStream req, rsp;
    req << 23 << 45;
    sendRequest("TestEvt1", req, rsp);

    int a, b;
    rsp >> a >> b;
    LLLOG("FSMA::Test1::onHeartBeat: %d, %d\n", a, b);

    stopHeartBeat();
}

bool FSMA::Test2::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    return true;
}

void FSMA::Test2::onHeartBeat()
{
    self()->stop();
}

}
