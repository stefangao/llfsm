//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "TestCase.h"
USING_NS_LL;

class TestCase2 : public TestCase
{
public:
    TestCase2();

protected:
    virtual ~TestCase2();
    virtual bool onInit();

    DECLARE_TESTCASE_FACTORY(TestCase2)
};

namespace test2 {

class FSMA: public FSM
{
protected:
    void onStart() override
    {
        FSM::onStart();
    }

    DECLARE_STATE_TABLE()
    DECLARE_TRANS_TABLE()

public:
    enum
    {
        DAEMON, TEST1, TEST2
    };

    class Daemon: public State
    {
    protected:
        virtual void onEnter() override
        {
            State::onEnter();

        }
        virtual void onExit() override
        {
            State::onExit();
        }

        DEFINE_STATE_FACTORY_OF_FSM(Daemon, FSMA)
    };

    class Test1: public State
    {
    protected:
        virtual void onEnter() override
        {
            State::onEnter();

            startHeartBeat(1000);
        }
        virtual void onExit() override
        {
            State::onExit();
        }
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override;
        virtual bool onRequestProc(const std::string& evtName, EvtStream& evtData, EvtStream& rspData) override;
        virtual void onHeartBeat() override;

        DEFINE_STATE_FACTORY_OF_FSM(Test1, FSMA)
    };

    class Test2: public State
    {
    protected:
        virtual void onEnter() override
        {
            State::onEnter();

            startHeartBeat(2000);
        }
        virtual void onExit() override
        {
            State::onExit();
        }
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override;
        virtual void onHeartBeat() override;

        DEFINE_STATE_FACTORY_OF_FSM(Test2, FSMA)
    };
};

} //namespace
