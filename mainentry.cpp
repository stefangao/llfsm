//============================================================================
// Name        : mainentry.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "lianli.h"
using namespace std;

//////////////////////////////////
class FSMA: public FSM
{
public:
    FSMA()
    {
        mX = 1;
    }

protected:
    DECLARE_STATE_TABLE()
    DECLARE_TRANS_TABLE()

public:
    enum
    {
        DAEMON, TEST1, TEST2, TEST3
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

        }
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_NEWINSTANCE(Daemon, FSMA)
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

        }
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_NEWINSTANCE(Test1, FSMA)
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

        }
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_NEWINSTANCE(Test2, FSMA)
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

        }
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_NEWINSTANCE(Test3, FSMA)
    };

};

BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, FSM::ROOT, FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    FSM::SFL_ZERO)
    STATE_ENTRY(TEST3,  Test2,  TEST1,     FSM::SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)

END_TRANS_TABLE()

//////////////////////////////////
class FSMB: public FSMA
{
public:
    FSMB()
    {
        mX = 2;
    }

    DECLARE_TRANS_TABLE()
};

BEGIN_TRANS_TABLE(FSMB, FSMA)

END_TRANS_TABLE()

//////////////////////////////////
class FSMC: public FSMB
{
public:
    FSMC()
    {
        mX = 3;
    }

    DECLARE_TRANS_TABLE()
};

BEGIN_TRANS_TABLE(FSMC, FSMB)

END_TRANS_TABLE()

//////////////////////////////////
class FSMD: public FSMC
{
public:
    FSMD()
    {
        mX = 4;
    }

    int & getValue()
    {
        static int ZERO = 0;
        return ZERO;
    }

    DECLARE_TRANS_TABLE()
};

BEGIN_TRANS_TABLE(FSMD, FSMC)

END_TRANS_TABLE()

//////////////////////////////////////////////////////////////////////////

int main()
{
    FSMA *a = new FSMA();
    a->create("TestFsmA");
    a->start();

    //FSMD *d = new FSMD();
    //d->printX();

    return 0;
}

