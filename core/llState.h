//============================================================================
// Name        : llState.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_STATE_H__
#define __LL_STATE_H__

#include <string>
#include "llConst.h"
#include "llEvtData.h"

NS_LL_BEGIN

class FSM;

class State
{
    friend class FSM;
public:
    State();
    bool sendEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);
    bool postEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);
    const char* getName() const;

protected:
    virtual ~State() {}
    static State* createInstance() {return nullptr;}

    virtual void onCreate();
    virtual void onEnter();
    virtual void onBeat();
    virtual void onExit();
    virtual void onDestroy();
    virtual bool onEventProc(const std::string& evtName, const EvtData& evtData);

protected:
    sid mID;
    FSM* mThisFSM;
    StateNode_t* mStateNode;
};

NS_LL_END

#endif
