# llfsm
It is a fsm whose states are tree structure.
A fsm has states which ared composed by hierarchy format.
A fsm is also a state type, which can be a part of another fsm.

the project namespace is lianli. we have several most basical classes, which are most frequently used in programming:
lianli::FSM  (llFSM.h .cpp)
lianli::State (llState.h .cpp)
lianli::Context (llContext.h .cpp)



it's an example of fsm below:

fsm1:
-------------------------------------------------------
                 ROOT (hidden state, non programmable) 
                 state0 (real root state, programmable)
                    |
       state1               state2   
         |                    |
     fsm2 (state3)       fsm3 (state4)
-------------------------------------------------------

if fsm1 is created in context1, then
context1 {fsm1}
fsm1 {fsm2, fsm3}

the uri of fsm2 is /context1/fsm1/fsm2
the uri of fsm3 is /context1/fsm1/fsm3

we can locate every fsm by its uri, which is unique in a context.
