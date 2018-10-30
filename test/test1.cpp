//============================================================================
// Name        : mainentry.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test1.h"

/////////// FSMA ///////////
BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, FSM::ROOT, FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    FSM::SFL_ZERO)
    STATE_ENTRY(TEST3,  Test3,  TEST1,     FSM::SFL_ACTIVE)
    STATE_ENTRY(TEST4,  Test3,  TEST2,     FSM::SFL_ACTIVE)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)

END_TRANS_TABLE()


/////////// FSMB ///////////
BEGIN_TRANS_TABLE(FSMB, FSMA)

END_TRANS_TABLE()

/////////// FSMC ///////////
BEGIN_TRANS_TABLE(FSMC, FSMB)

END_TRANS_TABLE()
