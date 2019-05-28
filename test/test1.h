//============================================================================
// Name        : test1.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "TestCase.h"
USING_NS_LL;

class TestCase1 : public TestCase
{
protected:
    virtual bool onInit();

    DECLARE_TESTCASE_FACTORY(TestCase1)
};

namespace test1 {
class Dog: public FSM
{
protected:
    virtual void onStart() override;

public:
    void snore()
    {
        LLLOG("z~~Z~~~\n");
    }

    void bark()
    {
        LLLOG("wang wang ~~\n");
    }

    void walk()
    {
        LLLOG("walk..\n");
    }

DECLARE_STATE_TABLE()
DECLARE_TRANS_TABLE()

public:
    enum {AWAKE, SLEEP};

    class Awake: public State
    {
    protected:
        virtual void onEnter() override;
        virtual void onExit() override;
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override;
        virtual void onHeartBeat() override;

    DEFINE_STATE_FACTORY_OF_FSM(Awake, Dog)
    };

    class Sleep: public State
    {
    protected:
        virtual void onEnter() override;
        virtual void onExit() override;
        virtual void onHeartBeat() override;

    DEFINE_STATE_FACTORY_OF_FSM(Sleep, Dog)
    };
};
};
