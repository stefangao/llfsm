//============================================================================
// Name        : llfsm.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "lianli.h"
using namespace std;

//////////////////////////////////
class FSMA : public FSM
{
public:
    FSMA()
    {
        mX = 1;
    }

protected:
    DECLARE_STATE_TABLE();
    DECLARE_TRANS_TABLE();

public:
	enum
	{
		DAEMON,
		TEST1
	};

    class Daemon : public State
    {
    protected:
        virtual void onEnter() override
        {

        }
        virtual void onExit() override
        {

        }
        virtual bool onEventProc(const std::string& evtName, const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_NEWINSTANCE(Daemon, FSMA)
    };

    class Test1 : public State
    {
    protected:
        virtual void onEnter() override
        {

        }
        virtual void onExit() override
        {

        }
        virtual bool onEventProc(const std::string& evtName, const EvtData& evtData) override
        {
            return true;
        }

        DECLARE_STATE_NEWINSTANCE(Test1, FSMA)
    };

};

BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON,  Daemon,    0,   SFL_ACTIVE)
    STATE_ENTRY(TEST1,   Test1,     0,   0)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)

END_TRANS_TABLE()

//////////////////////////////////
class FSMB : public FSMA
{
public:
    FSMB()
    {
        mX = 2;
    }

protected:
    DECLARE_TRANS_TABLE();

};

BEGIN_TRANS_TABLE(FSMB, FSMA)

END_TRANS_TABLE()

//////////////////////////////////
class FSMC : public FSMB
{
public:
    FSMC()
    {
        mX = 3;
    }

protected:
    DECLARE_TRANS_TABLE();

};

BEGIN_TRANS_TABLE(FSMC, FSMB)

END_TRANS_TABLE()

//////////////////////////////////
class FSMD : public FSMC
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

protected:
    DECLARE_TRANS_TABLE();

};

BEGIN_TRANS_TABLE(FSMD, FSMC)

END_TRANS_TABLE()

//////////////////////////////////////////////////////////////////////////

int main() {
    FSMA *a = new FSMA();
    a->create("TestFsmA");
    a->printX();



    //FSMD *d = new FSMD();
    //d->printX();

	return 0;
}

