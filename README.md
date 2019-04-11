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
the uri of fsm1 is context1:fsm1.

fsm2 and fsm3 are the states of fsm1. So their context are also context1.
The uri of fsm2 is context1:fsm2.
The uri of fsm3 is context1:fsm3.

The name of fsm must be unique within a context. So we can locate every fsm by its uri.

Usage:
=

1. How to build llfsm?
llfsm is a library, which can be used in executable program. So it is better to build test project and llfsm together. test is the executable program, which includes some test cases that will demonstrate how to use llfsm.

The commands to generate test project is:
 1) open terminal
 2) enter root directory
 3) cd build
 4) cmake ../test
 5) make
 
Now the executable program will be created in the folder test/bin and llfsm.a will be created in the folder lib meanwhile. We can run the program by
 ./llfsm_test
 
 You will see the output logs that show the fsm's progress.
 
 2. If you don't want generate test project, you just only do like this,
  1) open terminal
  2) enter root directory
  3) cd build
  4) cmake ..
  5) make
 
 Now only llfsm.a will be created in lib folder.
 
 3. How to generate test project for eclipse? Please see the following steps,
  1) open terminal
  2) enter root directory
  3) cd build
  4) cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=debug ../test
  5) make

 The you can open the project from eclipse and edit the code.


