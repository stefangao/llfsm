//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test5.h"

namespace test5
{
class Dog: public FSM
{
protected:
    void onStart() override
    {
        FSM::onStart();

        setEventHandler("TestEvt1", LL_BIND_EVENT(Dog::onEventProc, this));
        setRequestHandler("TestEvt1", LL_BIND_REQUEST(Dog::onRequestProc, this));

        //postEvent("TestEvt1");

        EvtStream data, rsp;
        data << 123;
        sendRequest("TestEvt1", data, rsp);
        int value;
        rsp >> value;
        LLLOG("Dog::onRequestProc: rsp=%d\n", value);
    }

    bool onEventProc(EvtStream& evtData)
    {
        LLLOG("Dog::onEventProc ===\n");
        return true;
    }

    bool onRequestProc(EvtStream& evtData, EvtStream& rspData)
    {
        int req;
        evtData >> req;
        LLLOG("Dog::onRequestProc: req=%d\n", req);
        rspData << req + 333;
        return true;
    }
};
}

bool TestCase5::onInit()
{
    FSM* fsm = LL_CREATE_FSM(test5::Dog, "dog", Context::DEFAULT);
    fsm->start();
    return true;
}
