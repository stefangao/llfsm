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

bool State::onRequestProc(const std::string& evtName, EvtStream& evtData, EvtStream& rspData)
{
    LLLOG("State::onRequestProc() : name=%s.%s\n", mThisFSM->getName().c_str(), getName());
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
            {
                mStateNode->hbCount++;
                onHeartBeat();
            }
        });
    }

    mStateNode->hbTimerID = setTimer(interval, [this](int tid, const void* userData) {
        if (isActive())
        {
            mStateNode->hbCount++;
            onHeartBeat();
        }
    });
    mStateNode->hbInterval = interval;
    mStateNode->hbCount = 0;
    return true;
}

void State::stopHeartBeat()
{
    if (mStateNode &&mStateNode->hbTimerID != -1)
    {
        killTimer(mStateNode->hbTimerID);
        mStateNode->hbTimerID = -1;
        mStateNode->hbInterval = -1;
        mStateNode->hbCount = 0;
    }
}

bool State::isHeartBeatOn()
{
    return mStateNode->hbTimerID != -1;
}

int State::getHeartBeatCount()
{
    return mStateNode->hbCount;
}

int State::getHeartBeatInterval()
{
    return mStateNode->hbInterval;
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

bool State::sendRequest(const std::string& evtName, const EvtStream& evtData, EvtStream& rspData)
{
    return mThisFSM->sendRequest(evtName, evtData, rspData);
}

void State::addEventHandler(const std::string& evtName, const EventHandler& eventHandler)
{
    mEventHandlerMap.insert(std::make_pair(evtName, eventHandler));
}

void State::addRequestHandler(const std::string& evtName, const RequestHandler& requestHandler)
{
    mRequestHandlerMap.insert(std::make_pair(evtName, requestHandler));
}

void State::removeEventHandler(const std::string& evtName)
{
    auto iter = mEventHandlerMap.find(evtName);
    if (iter != mEventHandlerMap.end())
    {
        mEventHandlerMap.erase(iter);
    }
}

void State::removeRequestHandler(const std::string& evtName)
{
    auto iter = mRequestHandlerMap.find(evtName);
    if (iter != mRequestHandlerMap.end())
    {
        mRequestHandlerMap.erase(iter);
    }
}

NS_LL_END
