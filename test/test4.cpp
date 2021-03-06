//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test4.h"
using namespace test4;

bool TestCase4::onInit()
{
    auto dog = LL_CREATE_FSM(Dog, "dog", Context::DEFAULT);
    mMainFSM = dog;
    return true;
}

namespace test4 {
  
BEGIN_STATE_TABLE(Dog)
    STATE_ENTRY(DAEMON, Daemon, S_ROOT,    SFL_ACTIVE)
    STATE_ENTRY(SLEEP,  Sleep,  DAEMON,    0)
    STATE_ENTRY(AWAKE,  Awake,  DAEMON,    SFL_ACTIVE)
    STATE_ENTRY(WALK,   Walk,   AWAKE,     SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(Dog, FSM)
    //TRANS_ENTRY(SLEEP, "TestEvt1", WALK)
    TRANS_ENTRY(WALK, "HelloEvt1", SLEEP)
END_TRANS_TABLE()

void Dog::Sleep::onEnter()
{
    State::onEnter();

    //startHeartBeat(3000);

    EvtStream data;
    std::string input = "000 hello world! 300 abc123\n";
    data << 23 << input;
    //postEvent("TestEvt1", data);
}

bool Dog::Sleep::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    std::stringstream ss;
    int a;
    std::string str;
    evtData >> a;
    evtData >> str;
    LLLOG("Dog::Sleep::onEventProc(): a=%d, str=%s\n", a, str.c_str());
    return true;
}

void Dog::Sleep::onHeartBeat()
{
    LLLOG("~ ~ ~\n");

    //postEvent("TestEvt1", data);
}

void Dog::Sleep::onExit()
{
    State::onExit();
}

void Dog::Awake::onEnter()
{
    State::onEnter();

    startHeartBeat(2000);
}

bool Dog::Awake::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    std::stringstream ss;
    int a;
    std::string str;
    evtData >> a >> str;
    LLLOG("Dog::Awake::onEventProc(): a=%d, str=%s\n", a, str.c_str());
    return true;
}

void Dog::Awake::onHeartBeat()
{
    LLLOG("wang...\n");
}

void Dog::Awake::onExit()
{
    State::onExit();
}

void Dog::Walk::onEnter()
{
    State::onEnter();

    //startHeartBeat(2000);
    auto fsm = dynamic_cast<FSM*>(this);
    if (fsm)
        fsm->postEvent("ToTestEvt");
}

bool Dog::Walk::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    LLLOG("Dog::Walk::onEventProc() : evtName=%s\n", evtName.c_str());
    return true;
}

void Dog::Walk::onHeartBeat()
{
    //postEvent("TestEvt2");
}

void Dog::Walk::onExit()
{
    State::onExit();
}

BEGIN_STATE_TABLE(Dog::Walk)
STATE_ENTRY(WDAEMON, WDaemon, S_ROOT, 0)
STATE_ENTRY(WTEST, WTest, WDAEMON, SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(Dog::Walk, FSM)
TRANS_ENTRY(WDAEMON, "TestEvt1", S_NONE)
TRANS_ENTRY(S_ROOT, "ToTestEvt", WTEST)
END_TRANS_TABLE()

}
