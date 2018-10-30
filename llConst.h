//============================================================================
// Name        : llConst.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_CONST_H__
#define __LL_CONST_H__

#include <vector>

#ifdef __cplusplus
    #define NS_LL_BEGIN                     namespace lianli {
    #define NS_LL_END                       }
    #define USING_NS_LL                     using namespace lianli
#else
    #define NS_LL_BEGIN
    #define NS_LL_END
    #define USING_NS_LL
#endif

NS_LL_BEGIN

#define LL_DEBUG

typedef int sid;

class State;
typedef State* (*StateFactoryFunc)();

typedef struct
{
    sid id;
    StateFactoryFunc createInstance;
    sid parent;
    unsigned int flag;
    const char* name;
} StateEntry_t;

typedef struct
{
    sid fromState;
    const char* eventName;
    sid toState;
    unsigned int flag;
} TransEntry_t;

typedef struct _StateNode_t
{
    const StateEntry_t *stateEntry;
    State *stateObject;

    std::vector<const _StateNode_t*> childNodes;
    sid activeChild;       //current active child state id

    int modeticks;         //the last ticks when onMode was triggered
    int modemaxticks;      //the tick count for onMode
    int modedelayticks;    //the first onMode time

    unsigned int sopFlag;  //flag indicate on which procedure the state is

} StateNode_t;

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
