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
USING_NS_LL;

//////////////////////////////////
class FSMA : public FSM
{
public:
    FSMA()
    {
        mX = 1;
    }

protected:
    DECLARE_TRANS_TABLE();

};

BEGIN_TRANS_TABLE(FSMA, FSM);

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

BEGIN_TRANS_TABLE(FSMB, FSMA);

//////////////////////////////////
class FSMC : public FSMB
{
public:
    FSMC()
    {
        //mX = 3;
    }

protected:
    //DECLARE_TRANS_TABLE();

};

//BEGIN_TRANS_TABLE(FSMC, FSMB);

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

BEGIN_TRANS_TABLE(FSMD, FSMC);

//////////////////////////////////////////////////////////////////////////

int main() {
    FSMD *d = new FSMD();
    d->printX();

	return 0;
}










