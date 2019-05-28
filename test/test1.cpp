//============================================================================
// Name        : test1.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test1.h"

bool TestCase1::onInit()
{
    FSM* fsm = LL_CREATE_FSM(test1::Dog, "dog", Context::DEFAULT);
    mMainFSM = fsm;
    return true;
}

namespace test1 {
BEGIN_STATE_TABLE(Dog)
    STATE_ENTRY(AWAKE, Awake, S_ROOT, 0)
    STATE_ENTRY(SLEEP, Sleep,  S_ROOT, SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(Dog, FSM)
    TRANS_ENTRY(SLEEP, "SoundEvt", AWAKE, TFL_TOPROC)
    TRANS_ENTRY(AWAKE, "SleepEvt", SLEEP)
END_TRANS_TABLE()

void Dog::onStart()
{
    delayPostEvent(3000, "SoundEvt");
}

void Dog::Awake::onEnter()
{
    LLLOG("[AWAKE enter]\n");
    startHeartBeat(2000);
}

bool Dog::Awake::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    if (evtName == "SoundEvt")
    {
        self()->bark();
    }
    return true;
}

void Dog::Awake::onHeartBeat()
{
    self()->walk();

    int hbCount = getHeartBeatCount();
    if (hbCount == 5)
        postEvent("SleepEvt");
}

void Dog::Awake::onExit()
{

}

void Dog::Sleep::onEnter()
{
    LLLOG("[SLEEP enter]\n");
    startHeartBeat(1000);
}

void Dog::Sleep::onHeartBeat()
{
    self()->snore();
}

void Dog::Sleep::onExit()
{

}

}
