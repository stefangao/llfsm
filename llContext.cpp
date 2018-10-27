//============================================================================
// Name        : llContext.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "llContext.h"

NS_LL_BEGIN

Context Context::DEFAULT = Context("defaultContext");

Context::Context(const std::string& name) :
        mName(name)
{

}

void Context::insert(const FSM* fsm)
{
    mFsmList.push_back(fsm);
}

void Context::remove(const FSM* fsm)
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

NS_LL_END
