#include "llFSM.h"

#include <iostream> //TBD
using namespace std;

NS_LL_BEGIN

void FSM::printX()
{
    const FSM* fsm = this;
    do
    {
        cout << fsm->getX() << endl;
        fsm = fsm->__getSuperBuilder();
    } while (fsm);

}

NS_LL_END
