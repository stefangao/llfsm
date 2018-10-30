//============================================================================
// Name        : llFSM.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_FSM_H__
#define __LL_FSM_H__

#include <string>
#include "llRef.h"
#include "llState.h"
#include "llContext.h"

NS_LL_BEGIN

class FSM: public State, public Ref
{
public:
    enum class S
    {
        IDLE, RUN, PAUSED, INVAL = -1,
    };

protected:
    virtual ~FSM();
    static const FSM* __getBuilder() {return nullptr;}
    virtual const FSM* __getSuperBuilder() const {return nullptr;}
    virtual const StateEntry_t* getStateTable() const {return nullptr;}
    virtual const TransEntry_t* getTransTable() const {return nullptr;}
    virtual int getStateCount() const {return 0;}
    virtual void setStateCount(int count) { }
    virtual int getTransCount() const {return 0;}
    virtual void setTransCount(int) { }

    const StateNode_t& getStateNode(sid sID) const;
    StateNode_t& getStateNode(sid sID);

    const State& getState(sid sID) const;
    bool enterState(sid sID, bool enterDefaultActive = false);
    bool exitState(sid sID);

public:
    FSM();
    bool create(const std::string& name, Context& context = Context::DEFAULT);
    bool start();
    bool pause();
    bool resume();
    bool stop();
    bool destroy();

    bool sendEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);
    bool postEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);

    inline const std::string& getName() const {return mName;};
    inline void setName(const std::string& name) {mName = name;}
    inline S getS() const {return mS;}

    bool isInvalid() const;
    bool isStateActive(sid sID) const;
    sid getActiveLeafState() const;
    int getStateLevel(sid sID) const;

    bool isStateInvalid(sid sID) const;
    bool switchState(sid dstState);

protected:
    virtual void onCreate(const Context& context);
    virtual void onStart();
    virtual void onPause();
    virtual void onResume();
    virtual void onStop();
    virtual void onDestroy(const Context& context);
    bool buildStateTree(sid parent);


    sid seekParent(sid sID, int level);
    sid seekCommonParent(sid sID1, sid sID2);

private:
    bool createInternal(const std::string& name, Context& context = Context::DEFAULT);
    void onCreateInternal(Context& context);

protected:
    std::string mName;
    S mS;
    Context *mContext;
    std::vector<StateNode_t> mStateNodeTable;
    StateNode_t mRootNode;

public:
    static const sid ROOT;
    static const sid INVAL;

    static const unsigned int SFL_ZERO;
    static const unsigned int SFL_ACTIVE;

    static const StateEntry_t ROOT_ENTRY;

    static const unsigned int SOP_ENTER;
    static const unsigned int SOP_BEAT;
    static const unsigned int SOP_EXIT;
};

NS_LL_END

#endif
