#ifndef __LLFSM_H__
#define __LLFSM_H__

#include "llState.h"
#include "llRef.h"

NS_LL_BEGIN

class FSM : public State, public Ref
{
protected:
    static const FSM* __getBuilder() {return nullptr;};
    virtual const FSM* __getSuperBuilder() const {return nullptr;};
    virtual int getX() const {return 0;};

public:
    void printX();
};

#define DECLARE_TRANS_TABLE() \
private: \
    static const FSM* __builder; \
    int mX; \
protected: \
    static  const FSM* __getBuilder(); \
    virtual const FSM* __getSuperBuilder() const override; \
    virtual int getX() const override; \

#define BEGIN_TRANS_TABLE(fsmClass, superClass) \
    const FSM* fsmClass::__builder = new fsmClass(); \
    const FSM* fsmClass::__getBuilder() \
		{ return fsmClass::__builder; } \
	const FSM* fsmClass::__getSuperBuilder() const \
	    { return superClass::__getBuilder(); } \
	int fsmClass::getX() const \
	    {return mX;}; \

NS_LL_END

#endif
