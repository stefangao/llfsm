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
    if (mThisFSM)
    {
        std::string msg;
        msg = std::string("State::~State(): ") + mThisFSM->getName() + "." + getName();
        LLLOG("test:aaa");
    }
}

void State::onInit()
{
    std::string msg;
    msg = std::string("State::onInit(): ") + mThisFSM->getName() + "." + getName();
    LLLOG(msg.c_str());
}

void State::onEnter()
{
    std::string msg;
    msg = std::string("State::onEnter(): ") + mThisFSM->getName() + "." + getName();
    LLLOG(msg.c_str());
}

bool State::onEventProc(const std::string& evtName, EvtData& evtData)
{
    std::string msg;
    msg = std::string("State::onEventProc(): ") + mThisFSM->getName() + "." + getName();
    LLLOG(msg.c_str());
    return true;
}

void State::onHeartBeat()
{
    std::string msg;
    msg = std::string("State::onHeartBeat(): ") + mThisFSM->getName() + "." + getName();
    LLLOG(msg.c_str());
}

void State::onExit()
{
    std::string msg;
    msg = std::string("State::onExit(): ") + mThisFSM->getName() + "." + getName();
    LLLOG(msg.c_str());
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
            onHeartBeat();
        });
    }

    mStateNode->hbTimerID = setTimer(interval, [this](int tid, const void* userData) {
        onHeartBeat();
    });

    return true;
}

void State::stopHeartBeat()
{
    if (mStateNode->hbTimerID != -1)
    {
        killTimer(mStateNode->hbTimerID);
        mStateNode->hbTimerID = -1;
    }
}

bool State::isHeatBeatOn()
{
    return mStateNode->hbTimerID != -1;
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
