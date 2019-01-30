//============================================================================
// Name        : llContext.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_CONTEXT_H__
#define __LL_CONTEXT_H__

#include <string>
#include <list>
#include "llConst.h"

NS_LL_BEGIN

class FSM;
class Context
{
    friend class FSM;
public:
    Context(const std::string& name);
    static const Context& getDefault() {return DEFAULT;};

    FSM* find(const std::string& name);

protected:
    virtual void add(FSM* fsm);
    virtual void remove(FSM* fsm);

protected:
    std::string mName;
    std::list<FSM*> mFsmList;

private:
    static Context DEFAULT;
};

NS_LL_END

#endif
