#ifndef __LLFSM_H__
#define __LLFSM_H__

#include <string>
#include "llState.h"
#include "llRef.h"
#include "llContext.h"

NS_LL_BEGIN

typedef struct
{
    int fromstate;
    const char* eventname;
    int tostate;
    unsigned int flag;
} TransEntry_t;

class FSM : public State, public Ref
{
public:
	enum class S
	{
        Stop,
        Start,
        Run,
        Pause,
		Max,
	};

protected:
    static const FSM* __getBuilder() {return nullptr;};
    virtual const FSM* __getSuperBuilder() const {return nullptr;};
    virtual const StateEntry_t* getStateTable() const {return nullptr;};
    virtual const TransEntry_t* getTransTable() const {return nullptr;};
    virtual int getStateCount() const {return 0;};
    virtual void setStateCount(int count) {};
    virtual int getTransCount() const {return 0;};
    virtual void setTransCount(int) {};

	virtual int getX() const {return 0;};

public:
	virtual ~FSM() {}
    bool create(const std::string& name, const Context& context = Context::DEFAULT);
    bool start();
    bool pause();
    bool resume();
    bool stop();
    bool destroy();

    bool sendEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);
    bool postEvent(const std::string& evtName, const EvtData& evtData = EvtData::EMPTY);

    void printX();

protected:
    virtual State* newInstance() override {return new FSM();};
    virtual void onCreate() {}
    virtual void onStart() {}
    virtual void onPause() {}
    virtual void onResume() {}
    virtual void onStop() {}
    virtual void onDestroy() {}

protected:
    std::string mName;
    S mS;
    const Context *mContext;
    StateNode_t *mStateNodeTable;
    int mStateNodeTableSize;
    StateNode_t* mRootNode;
};

#define DECLARE_STATE_TABLE() \
private: \
	static const StateEntry_t mStateEntries[]; \
    static int mStateCount; \
    virtual const StateEntry_t* getStateTable() const override; \
    virtual int getStateCount() const override; \
    virtual void setStateCount(int) override; \

#define BEGIN_STATE_TABLE(fsmClass) \
    int fsmClass::mStateCount = 0; \
    int fsmClass::getStateCount() const \
        { return mStateCount;}; \
    void fsmClass::setStateCount(int stateCount) \
        { mStateCount = stateCount;}; \
    const StateEntry_t* fsmClass::getStateTable() const \
        { return &fsmClass::mStateEntries[0]; } \
    const StateEntry_t fsmClass::mStateEntries[] = \
	    {

#define STATE_ENTRY(stateID, stateClass, parent, flag) \
    {stateID, new stateClass, parent, flag, #stateID},

#define END_STATE_TABLE() \
		{-1, 0, 0} \
	};

#define DECLARE_TRANS_TABLE() \
private: \
    static const FSM* __builder; \
    int mX; \
protected: \
    static  const FSM* __getBuilder(); \
    virtual const FSM* __getSuperBuilder() const override; \
    virtual int getX() const override; \
    static const TransEntry_t mTransEntries[]; \
	static int mTransCount; \
	virtual const TransEntry_t* getTransTable() const override; \
	virtual int getTransCount() const override; \
	virtual void setTransCount(int) override; \

#define BEGIN_TRANS_TABLE(fsmClass, superClass) \
    const FSM* fsmClass::__builder = new fsmClass(); \
    const FSM* fsmClass::__getBuilder() \
		{ return fsmClass::__builder; } \
	const FSM* fsmClass::__getSuperBuilder() const \
	    { return superClass::__getBuilder(); } \
	int fsmClass::getX() const \
	    {return mX;}; \
	int fsmClass::mTransCount = 0; \
	int fsmClass::getTransCount() const \
		{ return mTransCount;}; \
	void fsmClass::setTransCount(int transCount) \
		{ mTransCount = transCount;}; \
	const TransEntry_t* fsmClass::getTransTable() const \
		{ return &fsmClass::mTransEntries[0]; } \
	const TransEntry_t fsmClass::mTransEntries[] = \
		{ \

#define END_TRANS_TABLE() \
		{-1, nullptr, 0, 0}}; \

#define DECLARE_STATE_NEWINSTANCE(stateClass, fsmClass) \
protected: \
    virtual State* newInstance() override {return new stateClass();}; \
public: \
    inline fsmClass* me() {return (fsmClass*)mThisFSM;}; \

NS_LL_END

#endif
