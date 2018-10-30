//============================================================================
// Name        : mainentry.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "../core/lianli.h"
using namespace std;

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
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_FACTORY(Daemon, FSMA)
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
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_FACTORY(Test1, FSMA)
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
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_FACTORY(Test2, FSMA)
    };

    class Test3: public State
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
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_FACTORY(Test3, FSMA)
    };

    class Test4: public State
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
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_FACTORY(Test4, FSMA)
    };
};

//////////////////////////////////
class FSMB: public FSMA
{
public:
    FSMB()
    {
    }

    DECLARE_TRANS_TABLE()
};

//////////////////////////////////
class FSMC: public FSMB
{
public:
    FSMC()
    {
    }

    DECLARE_TRANS_TABLE()
};
