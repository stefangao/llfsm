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
    fsm->create("fsm2").start();

    EvtStream data_orig;
    data_orig << 234;
    int dataLen1 = data_orig.getDataLen();
    data_orig << 789;
    int dataLen2 = data_orig.getDataLen();

    int orig_p1 = data_orig.tellp();
    int orig_g1 = data_orig.tellg();

    EvtStream data = data_orig;

    int orig_p2 = data_orig.tellp();
    int orig_g2 = data_orig.tellg();

    int p1 = data.tellp();
    int g1 = data.tellg();

    int a, b;
    data >> a;

    int p2 = data.tellp();
    int g2 = data.tellg();

    data >> b;

    int p3 = data.tellp();
    int g3 = data.tellg();

    char buf[128] = {0};
    data.std::stringstream::read(buf, 128);

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
    //TRANS_ENTRY(TEST1, "TestEvt1", TEST2)
END_TRANS_TABLE()

bool FSMA::Test1::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    return true;
}

void FSMA::Test1::onHeartBeat()
{
    postEvent("TestEvt1");
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
