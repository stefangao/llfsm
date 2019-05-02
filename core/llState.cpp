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
#include "llTimer.h"

NS_LL_BEGIN

State::State()
{
    mID = -1;
    mThisFSM = nullptr;
    mStateNode = nullptr;
}

State::~State()
{
    stopHeartBeat();

    if (mThisFSM)
    {
        LLLOG("State::~State(): name=%s.%s\n", mThisFSM->getName().c_str(), getName());
    }
}

void State::onInit()
{
    LLLOG("State::onInit(): name=%s.%s\n", mThisFSM->getName().c_str(), getName());
}

void State::onEnter()
{
    LLLOG("State::onEnter(): name=%s.%s\n", mThisFSM->getName().c_str(), getName());
}

bool State::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    LLLOG("State::onEventProc() : name=%s.%s\n", mThisFSM->getName().c_str(), getName());
    return true;
}

void State::onHeartBeat()
{
    LLLOG("State::onHeartBeat(): name=%s.%s\n", mThisFSM->getName().c_str(), getName());
}

void State::onExit()
{
    LLLOG("State::onExit(): name=%s.%s\n", mThisFSM->getName().c_str(), getName());
}

bool State::sendEvent(const std::string& evtName, const EvtStream& evtData)
{
    return mThisFSM->sendEvent(evtName, evtData);
}

bool State::postEvent(const std::string& evtName, const EvtStream& evtData)
{
    return mThisFSM->postEvent(evtName, evtData);
}

const char* State::getName() const
{
    if (mStateNode)
        return mStateNode->stateEntry->name;

    return LL_STRING_EMPTY;
}

bool State::startHeartBeat(int interval, bool atOnce)
{
    LLASSERT(mStateNode, "state object is not created");

    if (mStateNode->hbTimerID != -1)
    {
        if (!atOnce && mStateNode->hbInterval == interval)
            return true;

        stopHeartBeat();
    }

    if (atOnce)
    {
        postCallback([this](const void* userData) {
            if (isActive())
                onHeartBeat();
        });
    }

    mStateNode->hbTimerID = setTimer(interval, [this](int tid, const void* userData) {
        if (isActive())
            onHeartBeat();
    });

    return true;
}

void State::stopHeartBeat()
{
    if (mStateNode &&mStateNode->hbTimerID != -1)
    {
        killTimer(mStateNode->hbTimerID);
        mStateNode->hbTimerID = -1;
    }
}

bool State::isHeatBeatOn()
{
    return mStateNode->hbTimerID != -1;
}

bool State::isActive()
{
    sid parent = mStateNode->stateEntry->parent;
    const StateNode_t& parentNode = mThisFSM->getStateNode(parent);
    return parentNode.activeChild == mID;
}

void State::processOfflineEvents()
{
    auto& events = mStateNode->offlineEvents;
    if (events.empty())
        return;

    postCallback([this, &events](const void* userData) {
        auto& evtPair = events.front();
        auto& evtName = evtPair.first;
        auto& evtData = evtPair.second;
        events.pop();
        sendEvent(evtName, *evtData);
        if (evtData)
            delete evtData;

        processOfflineEvents();
    });
}

NS_LL_END
