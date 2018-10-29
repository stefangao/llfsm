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
        mX_onlyForTest = 1;
    }

protected:
    void onStart() override
    {
        FSM::onStart();

        printX();
    }

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

        }
        virtual bool onEventProc(const std::string& evtName,
                const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_FACTORY(Test3, FSMA)
    };

};

BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, FSM::ROOT, FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    FSM::SFL_ZERO)
    STATE_ENTRY(TEST3,  Test3,  TEST1,     FSM::SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)

END_TRANS_TABLE()

//////////////////////////////////
class FSMB: public FSMA
{
public:
    FSMB()
    {
        mX_onlyForTest = 2;
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
        mX_onlyForTest = 3;
    }

    DECLARE_TRANS_TABLE()
};

BEGIN_TRANS_TABLE(FSMC, FSMB)

END_TRANS_TABLE()

//////////////////////////////////

class FSMX: public FSM
{
public:
    FSMX()
    {
        mX_onlyForTest = 1;
    }

protected:
    void onStart() override
    {
        FSM::onStart();
        printX();
    }

    DECLARE_STATE_TABLE()
    DECLARE_TRANS_TABLE()
public:
    enum
    {
        X0, X1, X2, X3
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

        DECLARE_STATE_FACTORY(Daemon, FSMX)
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

        DECLARE_STATE_FACTORY(Test1, FSMX)
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

        DECLARE_STATE_FACTORY(Test2, FSMX)
    };
};

BEGIN_STATE_TABLE(FSMX)
    STATE_ENTRY(X0, Daemon, FSM::ROOT, FSM::SFL_ACTIVE)
    STATE_ENTRY(X1, Test1,  X0,    FSM::SFL_ACTIVE)
    STATE_ENTRY(X2, Test2,  X0,    FSM::SFL_ZERO)
    STATE_ENTRY(X3, FSMA,   X1,    FSM::SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMX, FSM)

END_TRANS_TABLE()

//////////////////////////////////////////////////////////////////////////

class Test0
{

};

int main(int argc, const char * argv[])
{
    //FSM *fsm1 = new FSMA();
    //fsm1->create("TestFsmA");
    //fsm1->start();

    FSM *fsm2 = new FSMX();
    fsm2->create("TestFsmX");
    fsm2->start();

    //fsm1->destroy();
    fsm2->destroy();

    return 0;
}

