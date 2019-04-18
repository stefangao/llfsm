//============================================================================
// Name        : llContext.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "llContext.h"
#include "llFSM.h"

NS_LL_BEGIN

Context Context::DEFAULT = Context("defaultContext");

Context::Context(const std::string& name) :
        mName(name)
{

}

void Context::add(FSM* fsm)
{
    mFsmList.push_back(fsm);
}

void Context::remove(FSM* fsm)
{
    for (auto iter = mFsmList.begin(); iter != mFsmList.end(); iter++)
    {
        if (*iter == fsm)
        {
            mFsmList.erase(iter);
            break;
        }
    }
}

FSM* Context::find(const std::string& name)
{
    for (auto iter = mFsmList.begin(); iter != mFsmList.end(); iter++)
    {
        auto fsm = *iter;
        if (fsm->getName() == name)
        {
            return fsm;
        }
    }
    return nullptr;
}

void Context::postBcEvent(const std::string& evtName, const EvtData& evtData)
{
    for (auto fsm : mFsmList)
    {
        if (fsm->isBcEventSubscribed(evtName))
            fsm->postBcEvent(evtName, evtData);
    }
}

void Context::sendBcEvent(const std::string& evtName, const EvtData& evtData)
{
    for (auto fsm : mFsmList)
    {
        if (fsm->isBcEventSubscribed(evtName))
            fsm->sendBcEvent(evtName, evtData);
    }
}

NS_LL_END
