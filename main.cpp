//============================================================================
// Name        : mainentry.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test/test1.h"

int main(int argc, const char * argv[])
{
    FSM *fsm1 = new FSMA();
    fsm1->create("TestFsmA");
    fsm1->start();

    Utils::log("switch1");
    fsm1->switchState(FSMA::DAEMON);

    Utils::log("switch2");
    fsm1->switchState(FSMA::TEST1);

    Utils::log("switch3");
    fsm1->switchState(FSMA::TEST4);

    Utils::log("stop");
    fsm1->stop();
    fsm1->destroy();

    return 0;
}

