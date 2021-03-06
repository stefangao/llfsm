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
#include <queue>
#include <string>
#include <functional>

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
typedef unsigned char* pbyte;

const sid S_ROOT = 1000000;
const sid S_INVAL = -1;
const sid S_NONE = -1;

//state flag
const unsigned int SFL_ZERO = 0;
const unsigned int SFL_ACTIVE = 0x0001L;

//transfer flag
const unsigned int TFL_ZERO = 0x0000L;
const unsigned int TFL_TOPROC = 0x0001L;
const unsigned int TFL_OFFLINE = 0x0002L;
const unsigned int TFL_DYNAMIC = 0x1000L;

class State;
typedef State* (*StateFactoryFunc)();

typedef struct StateEntry_t
{
    sid id;
    StateFactoryFunc createInstance;
    sid parent;
    unsigned int flag;
    const char* name;
} StateEntry_t;

typedef struct TransEntry_t
{
    sid from;
    const char* event;
    sid to;
    unsigned int flag;
    TransEntry_t(sid from, const char* event, sid to, unsigned int flag = 0)
      : from(from), event(event), to(to), flag(flag) {}
} TransEntry_t;

class EvtStream;
typedef struct _StateNode_t
{
    const StateEntry_t *stateEntry;
    State *stateObject;

    std::vector<const _StateNode_t*> childNodes;
    sid activeChild;       //current active child state id

    int hbInterval;        //time interval every two successive heartbeat
    int hbTimerID;
    int hbCount;           //count of heart beat

    unsigned int sopFlag;  //flag indicate on which procedure the state is

    std::queue<std::pair<std::string, const EvtStream*>> offlineEvents;

} StateNode_t;

#define DECLARE_STATE_TABLE()\
private:\
    static const lianli::StateEntry_t mStateEntries[];\
    static int mStateCount;\
    virtual const lianli::StateEntry_t* getStateTable() const override;\
    virtual int getStateCount() const override;\
    virtual void setStateCount(int) override;\

#define BEGIN_STATE_TABLE(fsmClass)\
    int fsmClass::mStateCount = 0;\
    int fsmClass::getStateCount() const\
        { return mStateCount;};\
    void fsmClass::setStateCount(int stateCount)\
        { mStateCount = stateCount;};\
    const lianli::StateEntry_t* fsmClass::getStateTable() const\
        { return &fsmClass::mStateEntries[0]; }\
    const lianli::StateEntry_t fsmClass::mStateEntries[] =\
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
    static const lianli::TransEntry_t mTransEntries[];\
    static int mTransCount;\
    virtual const lianli::TransEntry_t* getTransTable() const override;\
    virtual int getTransCount() const override;\
    virtual void setTransCount(int) override;\

#define BEGIN_TRANS_TABLE(fsmClass, superClass)\
    const lianli::FSM* fsmClass::__builder = new fsmClass();\
    const lianli::FSM* fsmClass::__getBuilder()\
        { return fsmClass::__builder; }\
    const lianli::FSM* fsmClass::__getSuperBuilder() const\
        { return superClass::__getBuilder(); }\
    int fsmClass::mTransCount = 0;\
    int fsmClass::getTransCount() const\
        { return mTransCount;};\
    void fsmClass::setTransCount(int transCount)\
        { mTransCount = transCount;};\
    const lianli::TransEntry_t* fsmClass::getTransTable() const\
        { return &fsmClass::mTransEntries[0]; }\
    const lianli::TransEntry_t fsmClass::mTransEntries[] =\
        {{-1, nullptr, 0, 0}\

#define END_TRANS_TABLE()\
    ,{-1, nullptr, 0, 0}};\

#define TRANS_ENTRY ,lianli::TransEntry_t

#define DEFINE_STATE_FACTORY_OF_FSM(stateClass, fsmClass)\
public:\
    static lianli::State* createInstance()  {return new stateClass();}\
    inline fsmClass* self()  {return (fsmClass*)mThisFSM;};\

#define DEFINE_STATE_FACTORY(stateClass)\
public:\
    static lianli::State* createInstance()  {return new stateClass();}\

typedef std::function<bool(EvtStream& evtData)> EventHandler;
typedef std::function<bool(EvtStream& evtData, EvtStream& retData)> RequestHandler;

#define LL_BIND_EVENT(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define LL_BIND_REQUEST(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)

NS_LL_END

#endif
