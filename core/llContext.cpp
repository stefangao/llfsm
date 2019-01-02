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

NS_LL_END
