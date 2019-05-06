//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test3.h"
using namespace test3;

TestCase3::TestCase3()
{

}

TestCase3::~TestCase3()
{

}

bool TestCase3::onInit()
{
    FSM *fsm = new FSMA();
    fsm->create("fsm3").start();
    fsm->subscribeBcEvent("TestEvt1");


    EvtStream data;
    std::string input = "000 hello world! 300 abc123";
    data << 23 << input;
    fsm->sendBcEvent("TestEvt1", data);

    return true;
}

namespace test3 {
/////////// FSMA ///////////
BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, S_ROOT,    SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    0)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)
    TRANS_ENTRY(TEST1, "TestEvt1", S_NONE)
    TRANS_ENTRY(TEST1, "MyDelayEvt", S_NONE)
    //TRANS_ENTRY(TEST2, "TestEvt2", TEST1)
END_TRANS_TABLE()

bool FSMA::Test1::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    std::stringstream ss;
    int a;
    std::string str;
    evtData >> a;
    evtData >> str;
    ss << "Test1 Proc: a=" << a << " str=" << str;
    LLLOG(ss.str().c_str());
    return true;
}

void FSMA::Test1::onHeartBeat()
{
    EvtStream data;

    std::string input = "000 hello world! 300";
    data << 23 << input;

    //postEvent("TestEvt1", data);
}

bool FSMA::Test2::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    std::stringstream ss;
    int a;
    std::string str;
    evtData >> a >> str;
    ss << "Test2 Proc: a=" << a << " str=" << str;
    LLLOG(ss.str().c_str());

    return true;
}

void FSMA::Test2::onHeartBeat()
{
    postEvent("TestEvt2");
}

}
