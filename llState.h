#ifndef __LLSTATE_H__
#define __LLSTATE_H__

#include <string>
#include <vector>
#include "llConst.h"
#include "llEvtData.h"

NS_LL_BEGIN

class FSM;
class State;

typedef int sid;

typedef struct
{
	sid id;
    State* object;
    int parent;
    unsigned int flag;
    const char* name;
} StateEntry_t;

typedef struct _StateNode_t
{
	const StateEntry_t *stateEntry;
	State *stateObject;

	std::vector<const _StateNode_t*> childNodes;
	sid activeChild;       //current active child state id

	int modeticks;         //the last ticks when onMode was triggered
	int modemaxticks;      //the tick count for onMode
	int modedelayticks;    //the first onMode time

	int sopflag;           //flag indicate on which procedure the state is

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
