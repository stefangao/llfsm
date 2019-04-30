//============================================================================
// Name        : test4.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "TestCase.h"
USING_NS_LL;

class TestCase4 : public TestCase
{
protected:
    virtual bool onInit();

    DECLARE_TESTCASE_FACTORY(TestCase4)
};

namespace test4 {

class Dog: public FSM
{
public:
    Dog()
    {
    }

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
        DAEMON, SLEEP, AWAKE, WALK
    };

    class Daemon: public State
    {
    protected:
        virtual void onEnter() override
        {
            State::onEnter();
        }

        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
        {
            LLLOG("Dog::Daemon::onEventProc evtName=%s\n", evtName.c_str());
            return true;
        }

        virtual void onExit() override
        {
            State::onExit();
        }

        DEFINE_STATE_FACTORY_OF_FSM(Daemon, Dog)
    };

    class Sleep : public State
    {
    protected:
        virtual void onEnter() override;
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override;
        virtual void onHeartBeat() override;
        virtual void onExit() override;

        DEFINE_STATE_FACTORY_OF_FSM(Sleep, Dog)
    };

    class Awake: public State
    {
    protected:
        virtual void onEnter() override;
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override;
        virtual void onHeartBeat() override;
        virtual void onExit() override;

        DEFINE_STATE_FACTORY_OF_FSM(Awake, Dog)
    };

    class Walk : public FSM
    {
    protected:
        virtual void onEnter() override;
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override;
        virtual void onHeartBeat() override;
        virtual void onExit() override;

        DEFINE_STATE_FACTORY_OF_FSM(Walk, Dog)

    public:
        enum
        {
            WDAEMON, WTEST
        };

        class WDaemon : public State
        {
        protected:
            virtual void onEnter() override
            {
                State::onEnter();
            }

            virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
            {
                LLLOG("Walk::WDaemon::onEventProc evtName=%s\n", evtName.c_str());
                return true;
            }

            virtual void onExit() override
            {
                State::onExit();
            }

            DEFINE_STATE_FACTORY_OF_FSM(WDaemon, Walk)
        };

        class WTest : public State
        {
        protected:
            virtual void onEnter() override
            {
                State::onEnter();

                self()->getParent()->postEvent("HelloEvt1");
            }
            virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
            {
                return true;
            }
            virtual void onExit() override
            {
                State::onExit();
            }

            DEFINE_STATE_FACTORY_OF_FSM(WTest, Walk)
        };

        DECLARE_STATE_TABLE()
        DECLARE_TRANS_TABLE()
    };
};

} //namespace
