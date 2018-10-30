//============================================================================
// Name        : llState.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "llState.h"
#include "llFSM.h"
#include "llUtils.h"

NS_LL_BEGIN

State::State()
{
    mID = -1;
    mThisFSM = nullptr;
    mStateNode = nullptr;
}

void State::onCreate()
{
    std::string msg;
    msg = std::string("State::onCreate(): ") + mThisFSM->getName() + "." + getName();
    Utils::log(msg);
}

void State::onEnter()
{
    std::string msg;
    msg = std::string("State::onEnter(): ") + mThisFSM->getName() + "." + getName();
    Utils::log(msg);
}

bool State::onEventProc(const std::string& evtName, const EvtData& evtData)
{
    return true;
}

void State::onBeat()
{
}

void State::onExit()
{
    std::string msg;
    msg = std::string("State::onExit(): ") + mThisFSM->getName() + "." + getName();
    Utils::log(msg);
}

void State::onDestroy()
{

}

bool State::sendEvent(const std::string& evtName, const EvtData& evtData)
{
    return mThisFSM->sendEvent(evtName, evtData);
}

bool State::postEvent(const std::string& evtName, const EvtData& evtData)
{
    return mThisFSM->postEvent(evtName, evtData);
}

const char* State::getName() const
{
    if (mStateNode)
        return mStateNode->stateEntry->name;

    return LL_STRING_EMPTY;
}

NS_LL_END
