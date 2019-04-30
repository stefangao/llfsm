//============================================================================
// Name        : test1.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <sstream>
#include "TestCase.h"
USING_NS_LL;

class TestCase1 : public TestCase
{
public:
    TestCase1();

protected:
    virtual ~TestCase1();
    virtual bool onInit();

    DECLARE_TESTCASE_FACTORY(TestCase1)
};

namespace test1 {

class FSMA: public FSM
{
public:
    FSMA()
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
        DAEMON, TEST1, TEST2, TEST3, TEST4
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
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
        {
            State::onEventProc(evtName, evtData);
            return true;
        }

        DEFINE_STATE_FACTORY_OF_FSM(Daemon, FSMA)
    };

    class Test1: public State
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
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
        {
            State::onEventProc(evtName, evtData);
            return true;
        }

        DEFINE_STATE_FACTORY_OF_FSM(Test1, FSMA)
    };

    class Test2: public State
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
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
        {
            State::onEventProc(evtName, evtData);
            return true;
        }

        DEFINE_STATE_FACTORY_OF_FSM(Test2, FSMA)
    };

    class Test3: public State
    {
    protected:
        virtual void onEnter() override
        {
            State::onEnter();

            //startHeartBeat(2000);
        }
        virtual void onExit() override
        {
            State::onExit();
        }
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
        {
            State::onEventProc(evtName, evtData);
            return true;
        }

        DEFINE_STATE_FACTORY_OF_FSM(Test3, FSMA)
    };

    class Test4: public State
    {
    protected:
        virtual void onEnter() override
        {
            State::onEnter();

            //startHeartBeat(3000, true);
        }
        virtual void onExit() override
        {
            State::onExit();
        }
        virtual bool onEventProc(const std::string& evtName, EvtStream& evtData) override
        {
            State::onEventProc(evtName, evtData);

            int x;
            float f;
            std::string str;
            evtData >> x >> str >> f;
            std::stringstream msg;
            msg << "Test4::onEventProc x=" << x << " f=" << f << " str=" << str <<  std::endl;
            LLLOG(msg.str().c_str());
            return true;
        }

        DEFINE_STATE_FACTORY_OF_FSM(Test4, FSMA)
    };
};

} //namespace
