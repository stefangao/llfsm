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

protected:
    void insert(const FSM* fsm);
    void remove(const FSM* fsm);

protected:
    std::string mName;
    std::list<const FSM*> mFsmList;

public:
    static Context DEFAULT;
};

NS_LL_END

#endif
