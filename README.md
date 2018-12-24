# llfsm
It is a fsm whose states are tree structure.
A fsm has states which are composed by hierarchy format.
A fsm is also a state type, which can be a part of another fsm.

The project namespace is lianli. We have several basical classes, which are most frequently used in programming:
lianli::FSM  (llFSM.h .cpp)
lianli::State (llState.h .cpp)
lianli::Context (llContext.h .cpp)



It's an example of fsm below:

fsm1:
-------------------------------------------------------
                 ROOT (hidden state, non programmable) 
                 state0 (real root state, programmable)
                    |
       state1               state2   
         |                    |
     fsm2 (state3)       fsm3 (state4)
-------------------------------------------------------

If fsm1 is created in context1, then
the uri of fsm1 is context1:fsm1

fsm2 and fsm3 are the states of fsm1. So their context are also context1.
The uri of fsm2 is context1:fsm2
The uri of fsm3 is context1:fsm3

The name of fsm must be unique within a context. So we can locate every fsm by its uri.
