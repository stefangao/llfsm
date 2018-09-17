#ifndef __LLSTATE_H__
#define __LLSTATE_H__

#include <string>
#include "llConst.h"
#include "llEvtData.h"

NS_LL_BEGIN

class FSM;
class State;

typedef struct
{
    int id;
    State* object;
    int parent;
    unsigned int flag;
    const char* name;
} StateEntry_t;

typedef struct _CNTT_STATENODE_T
{
	const StateEntry_t    *stateEntry;
	State            *stateObject;

	int              activeChild;       //current active child state

	int              modeticks;         //the last ticks when onMode was triggered
	int              modemaxticks;      //the tick count for onMode
	int              modedelayticks;    //the first onMode time

	int              sopflag;           //状态当前所处的过程state on which procedure

} StateNode_t;

class State
{
	friend class FSM;
public:
	State();

    bool sendEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);
    bool postEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);

protected:
	virtual ~State() {}
	virtual State* newInstance() = 0;
    virtual void onEnter() {}
    virtual void onBeat() {}
    virtual void onExit() {}
    virtual bool onEventProc(const std::string& evtName, const EvtData& evtData) {return true;}

protected:
    int mID;
    FSM* mThisFSM;
};

NS_LL_END

#endif
