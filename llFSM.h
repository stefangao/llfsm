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
#include "llState.h"
#include "llRef.h"
#include "llContext.h"

NS_LL_BEGIN

typedef struct
{
    sid fromState;
    const char* eventName;
    sid toState;
    unsigned int flag;
} TransEntry_t;

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

    StateNode_t& getStateNode(sid sID);
    const State& getState(sid sID) const;
    bool enterState(sid sID);

    virtual int getX_onlyForTest() const {return 0;}

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

    void printX();

protected:
    virtual void onCreate(const Context& context);
    virtual void onStart();
    virtual void onPause();
    virtual void onResume();
    virtual void onStop();
    virtual void onDestroy(const Context& context);
    bool buildStateTree(sid parent);

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

#define DECLARE_STATE_TABLE()\
private:\
    static const StateEntry_t mStateEntries[];\
    static int mStateCount;\
    virtual const StateEntry_t* getStateTable() const override;\
    virtual int getStateCount() const override;\
    virtual void setStateCount(int) override;\
public:\
    static State* createInstance();\

#define BEGIN_STATE_TABLE(fsmClass)\
    int fsmClass::mStateCount = 0;\
    int fsmClass::getStateCount() const\
        { return mStateCount;};\
    void fsmClass::setStateCount(int stateCount)\
        { mStateCount = stateCount;};\
    const StateEntry_t* fsmClass::getStateTable() const\
        { return &fsmClass::mStateEntries[0]; }\
    State* fsmClass::createInstance() {return new fsmClass();}\
    const StateEntry_t fsmClass::mStateEntries[] =\
        {

#define STATE_ENTRY(stateID, stateClass, parent, flag)\
    {stateID, stateClass::createInstance, parent, flag, #stateID},

#define END_STATE_TABLE()\
    {-1, 0, 0}};\

#define DECLARE_TRANS_TABLE()\
private:\
    static const FSM* __builder;\
    int mX_onlyForTest;\
protected:\
    static  const FSM* __getBuilder();\
    virtual const FSM* __getSuperBuilder() const override;\
    virtual int getX_onlyForTest() const override;\
    static const TransEntry_t mTransEntries[];\
    static int mTransCount;\
    virtual const TransEntry_t* getTransTable() const override;\
    virtual int getTransCount() const override;\
    virtual void setTransCount(int) override;\

#define BEGIN_TRANS_TABLE(fsmClass, superClass)\
    const FSM* fsmClass::__builder = new fsmClass();\
    const FSM* fsmClass::__getBuilder()\
        { return fsmClass::__builder; }\
    const FSM* fsmClass::__getSuperBuilder() const\
        { return superClass::__getBuilder(); }\
    int fsmClass::getX_onlyForTest() const\
        {return mX_onlyForTest;};\
    int fsmClass::mTransCount = 0;\
    int fsmClass::getTransCount() const\
        { return mTransCount;};\
    void fsmClass::setTransCount(int transCount)\
        { mTransCount = transCount;};\
    const TransEntry_t* fsmClass::getTransTable() const\
        { return &fsmClass::mTransEntries[0]; }\
    const TransEntry_t fsmClass::mTransEntries[] =\
        {\

#define END_TRANS_TABLE()\
    {-1, nullptr, 0, 0}};\

#define DECLARE_STATE_FACTORY(stateClass, fsmClass)\
public:\
    static State* createInstance() {return new stateClass();}\
    inline fsmClass* me() {return (fsmClass*)mThisFSM;};\

NS_LL_END

#endif
