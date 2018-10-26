//============================================================================
// Name        : llFSM.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "llFSM.h"
#include "llUtils.h"
#include <iostream> //TBD

NS_LL_BEGIN

const sid FSM::INVAL = -1;
const sid FSM::ROOT = 1000000;

const unsigned int FSM::SFL_ZERO = 0;
const unsigned int FSM::SFL_ACTIVE = 0x0001L;

const unsigned int FSM::SOP_ENTER = 0x01;
const unsigned int FSM::SOP_BEAT = 0x02;
const unsigned int FSM::SOP_EXIT = 0x04;

const StateEntry_t FSM::ROOT_ENTRY = {FSM::ROOT, nullptr, FSM::INVAL, FSM::SFL_ZERO, "FSM::ROOT"};

FSM::FSM()
{
    mContext = nullptr;
}

FSM::~FSM()
{

}

bool FSM::create(const std::string& name, const Context& context)
{
    mContext = &context;
    mName = name;

    mRootNode.stateObject = nullptr;
    mRootNode.stateEntry = &ROOT_ENTRY;
    mRootNode.activeChild = FSM::INVAL;

    const StateEntry_t* stateEntry = getStateTable();
    if (stateEntry == nullptr)
        return false; //no state table is also a reasonable case

    int stateCount = getStateCount();
    if (stateCount == 0)
    {
        while (stateEntry++->id != -1)
            stateCount++;

        LLASSERT(stateCount > 0, "error");

        mStateNodeTable.resize(stateCount);

        stateEntry = getStateTable();
        for (int i = 0; i < stateCount; i++)
        {
            LLASSERT(stateEntry->id == i, "State ID is disordered!");
            mStateNodeTable[i].stateEntry = stateEntry;
            mStateNodeTable[i].stateObject = stateEntry->object->newInstance();
            mStateNodeTable[i].stateObject->mThisFSM = this;
            mStateNodeTable[i].stateObject->mID = stateEntry->id;
            mStateNodeTable[i].stateObject->mStateNode = &mStateNodeTable[i];
            stateEntry++;
        }
        setStateCount(stateCount);
    }
    buildStateTree(FSM::ROOT);

    int transCount = getTransCount();
    if (transCount == 0)
    {
        const FSM* builder = this;
        do
        {
            const TransEntry_t* transEntry = builder->getTransTable();
            if (transEntry != NULL)
            {
                while (transEntry++->fromState != -1)
                    transCount++;
            }
            builder = builder->__getSuperBuilder();
        } while (builder);

        if (transCount > 0)
        {
            setTransCount(transCount);
        }
    }

    onCreate();

    for (int i = 0; i < stateCount; i++)
    {
        mStateNodeTable[i].stateObject->onCreate();
    }

    return true;
}

StateNode_t& FSM::getStateNode(sid sID)
{
    if (sID == FSM::ROOT)
        return mRootNode;

    LLASSERT(sID >= 0 && sID < (int)mStateNodeTable.size(), "error");

    return mStateNodeTable[sID];
}

bool FSM::buildStateTree(sid parent)
{
    StateNode_t& parentNode = getStateNode(parent);
    parentNode.activeChild = FSM::INVAL;
    for (int i = 0; i < (int) mStateNodeTable.size(); i++)
    {
        StateNode_t& stateNode = mStateNodeTable[i];
        if (stateNode.stateEntry->parent == parent)
        {
            parentNode.childNodes.push_back(&stateNode);
            if (!buildStateTree(stateNode.stateEntry->id))
                return false;
        }
    }
    return true;
}

bool FSM::enterState(sid sID)
{
    StateNode_t& stateNode = getStateNode(sID);

    stateNode.activeChild = FSM::INVAL;
    if (stateNode.stateObject && (stateNode.sopFlag & SOP_ENTER) != SOP_ENTER)
    {
        stateNode.sopFlag |= SOP_ENTER;
        stateNode.stateObject->onEnter();
        stateNode.sopFlag &= ~SOP_ENTER;
        //stateNode.modeticks = cntt_GetTickCount() - statenode->modemaxticks + statenode->modedelayticks;
    }

    if (stateNode.activeChild == FSM::INVAL)
    {
        for (auto childNode : stateNode.childNodes)
        {
            if (childNode->stateEntry->flag & SFL_ACTIVE)
            {
                stateNode.activeChild = childNode->stateEntry->id;
                break;
            }
        }
        if (stateNode.activeChild != FSM::INVAL)
        {
            enterState(stateNode.activeChild);
        }
    }

    return true;
}

bool FSM::start()
{
    if (!enterState(FSM::ROOT))
        return false;

    onStart();
    return true;
}

bool FSM::pause()
{
    return true;
}

bool FSM::resume()
{
    return true;
}

bool FSM::stop()
{
    return true;
}

bool FSM::destroy()
{
    return true;
}

bool FSM::sendEvent(const std::string& evtName, const EvtData& evtData)
{
    return true;
}

bool FSM::postEvent(const std::string& evtName, const EvtData& evtData)
{
    return true;
}

void FSM::onCreate()
{
    Utils::log(getName() + ".onCreate");
}

void FSM::onStart()
{
    Utils::log(getName() + ".onStart");
}

void FSM::onPause()
{
    Utils::log(getName() + ".onPause");
}

void FSM::onResume()
{
    Utils::log(getName() + ".onResume");
}

void FSM::onStop()
{
    Utils::log(getName() + ".onStop");
}

void FSM::onDestroy()
{
    Utils::log(getName() + ".onDestroy");
}

void FSM::printX()
{
    const FSM* fsm = this;
    do
    {
        std::cout << fsm->getX_onlyForTest() << std::endl;
        fsm = fsm->__getSuperBuilder();
    } while (fsm);
}

NS_LL_END
