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
#include <map>
#include "llConst.h"
#include "llEvtStream.h"

NS_LL_BEGIN

class FSM;

class State
{
    friend class FSM;
public:
    State();
    bool sendEvent(const std::string& evtName, const EvtStream& evtData = EvtStream::EMPTY);
    bool postEvent(const std::string& evtName, const EvtStream& evtData = EvtStream::EMPTY);
    const char* getName() const;

    bool sendRequest(const std::string& evtName, const EvtStream& evtData, EvtStream& rspData = (EvtStream&)EvtStream::EMPTY);

    bool startHeartBeat(int interval, bool atOnce = false);
    void stopHeartBeat();
    bool isHeartBeatOn();
    int  getHeartBeatCount();
    int  getHeartBeatInterval();

    bool isActive();

    void addEventHandler(const std::string& evtName, const EventHandler& eventHandler);
    void addRequestHandler(const std::string& evtName, const RequestHandler& requestHandler);

    void removeEventHandler(const std::string& evtName);
    void removeRequestHandler(const std::string& evtName);

protected:
    virtual ~State();
    static State* createInstance() {return nullptr;}
    void processOfflineEvents();

    virtual void onInit();
    virtual void onEnter();
    virtual void onHeartBeat();
    virtual bool onEventProc(const std::string& evtName, EvtStream& evtData);
    virtual bool onRequestProc(const std::string& evtName, EvtStream& evtData, EvtStream& rspData);
    virtual void onExit();

protected:
    sid mID;
    FSM* mThisFSM;
    StateNode_t* mStateNode;
    std::map<std::string, EventHandler> mEventHandlerMap;
    std::map<std::string, RequestHandler> mRequestHandlerMap;
};

NS_LL_END

#endif
