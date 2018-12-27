//============================================================================
// Name        : llFSM.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "llFSM.h"

#include "llTimer.h"
#include "llUtils.h"

NS_LL_BEGIN

const unsigned int FSM::SOP_ENTER = 0x01;
const unsigned int FSM::SOP_BEAT = 0x02;
const unsigned int FSM::SOP_EXIT = 0x04;

const StateEntry_t FSM::ROOT_ENTRY = {S_ROOT, nullptr, S_INVAL, SFL_ZERO, "FSM::ROOT"};

FSM::FSM()
{
    mContext = nullptr;
    mS = S::INVAL;
    mName = LL_STRING_EMPTY;
}

FSM::~FSM()
{

}

bool FSM::create(const std::string& name, Context& context)
{
    if (!createInternal(name, context))
        return false;

    onCreateInternal(context);
    return true;
}

bool FSM::createInternal(const std::string& name, Context& context)
{
    const StateEntry_t* stateEntry = getStateTable();
    if (stateEntry == nullptr)
        return false;

    mContext = &context;
    mName = name;

    mRootNode.stateObject = nullptr;
    mRootNode.stateEntry = &ROOT_ENTRY;
    mRootNode.activeChild = S_INVAL;

    int stateCount = getStateCount();
    if (stateCount == 0)
    {
        while (stateEntry++->id != S_INVAL)
            stateCount++;

        LLASSERT(stateCount > 0, "error");
        setStateCount(stateCount);
    }

    if ((int)mStateNodeTable.size() != stateCount)
    {
        mStateNodeTable.resize(stateCount);

        stateEntry = getStateTable();
        for (int i = 0; i < stateCount; i++)
        {
            LLASSERT(stateEntry->id == i, "State ID is disordered!");
            mStateNodeTable[i].stateEntry = stateEntry;
            mStateNodeTable[i].stateObject = stateEntry->createInstance();
            mStateNodeTable[i].stateObject->mThisFSM = this;
            mStateNodeTable[i].stateObject->mID = stateEntry->id;
            mStateNodeTable[i].stateObject->mStateNode = &mStateNodeTable[i];
            stateEntry++;
        }
    }
    buildStateTree(S_ROOT);

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

    mS = S::IDLE;
    mContext->add(this);

    for (int i = 0; i < stateCount; i++)
    {
        FSM* fsm = dynamic_cast<FSM*>(mStateNodeTable[i].stateObject);
        if (fsm)
        {
            std::string fsmName = fsm->getName();
            if (fsmName.empty())
                fsmName = name + "." + mStateNodeTable[i].stateObject->getName();

            fsm->createInternal(fsmName, context);
        }
    }

    return true;
}

void FSM::onCreateInternal(Context& context)
{
    onCreate(context);

    int stateCount = getStateCount();
    for (int i = 0; i < stateCount; i++)
    {
        mStateNodeTable[i].stateObject->onInit();
    }

    for (int i = 0; i < stateCount; i++)
    {
        FSM* fsm = dynamic_cast<FSM*>(mStateNodeTable[i].stateObject);
        if (fsm)
        {
            fsm->onCreateInternal(context);
        }
    }
}

const StateNode_t& FSM::getStateNode(sid sID) const
{
    if (sID == S_ROOT)
        return mRootNode;

    LLASSERT(sID >= 0 && sID < (int)mStateNodeTable.size(), "error");

    return mStateNodeTable[sID];
}

StateNode_t& FSM::getStateNode(sid sID)
{
    if (sID == S_ROOT)
        return mRootNode;

    LLASSERT(sID >= 0 && sID < (int)mStateNodeTable.size(), "error");

    return mStateNodeTable[sID];
}

const State& FSM::getState(sid sID) const
{
    LLASSERT(sID >= 0 && sID < (int)mStateNodeTable.size(), "error");
    LLASSERT(mStateNodeTable[sID].stateObject, "error");

    return *mStateNodeTable[sID].stateObject;
}

bool FSM::buildStateTree(sid parent)
{
    StateNode_t& parentNode = getStateNode(parent);
    parentNode.activeChild = S_INVAL;
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

bool FSM::enterState(sid sID, bool enterDefaultActive)
{
    StateNode_t& stateNode = getStateNode(sID);
    sid parent = stateNode.stateEntry->parent;
    if (parent != S_INVAL)
    {
        StateNode_t& parentNode = getStateNode(parent);
        if (parentNode.activeChild == sID)
        {
            if (!enterDefaultActive)
                return true;
            else
                parentNode.activeChild = S_INVAL;
        }

        LLASSERT(parentNode.activeChild == S_INVAL, "the state must be inactive");
        parentNode.activeChild = sID;
    }

    if (stateNode.stateObject && (stateNode.sopFlag & SOP_ENTER) != SOP_ENTER)
    {
        stateNode.sopFlag |= SOP_ENTER;
        stateNode.stateObject->onEnter();
        stateNode.sopFlag &= ~SOP_ENTER;

        FSM* fsm = dynamic_cast<FSM*>(stateNode.stateObject);
        if (fsm)
        {
            switch (fsm->getS())
            {
                case S::IDLE:
                    fsm->start();
                    break;

                case S::PAUSED:
                    fsm->resume();
                    break;

                default:
                    break;
            }
        }
    }

    if (enterDefaultActive && stateNode.activeChild == S_INVAL)
    {
        sid activeChild = S_INVAL;
        for (auto childNode : stateNode.childNodes)
        {
            if (childNode->stateEntry->flag & SFL_ACTIVE)
            {
                activeChild = childNode->stateEntry->id;
                break;
            }
        }
        if (activeChild != S_INVAL)
        {
            enterState(activeChild, enterDefaultActive);
        }
    }

    return true;
}

bool FSM::exitState(sid sID)
{
    if (!isStateActive(sID))
        return true;

    sid activeLeaf = sID;
    StateNode_t* stateNode = &getStateNode(sID);
    while (stateNode->activeChild != S_INVAL)
    {
        activeLeaf = stateNode->activeChild;
        stateNode = &getStateNode(activeLeaf);
    }

    while (stateNode->stateEntry->id != S_ROOT)
    {
        if (stateNode->stateObject && (stateNode->sopFlag & SOP_EXIT) != SOP_EXIT)
        {
            stateNode->sopFlag |= SOP_EXIT;
            stateNode->stateObject->onExit();
            stateNode->sopFlag &= ~SOP_EXIT;
        }

        sid parent = stateNode->stateEntry->parent;
        StateNode_t* parentNode = &getStateNode(parent);
        parentNode->activeChild = S_INVAL;

        if (stateNode->stateEntry->id == sID)
            break;

        stateNode = parentNode;
    }

    return true;
}

bool FSM::start()
{
    if (!enterState(S_ROOT, true))
        return false;

    mS = S::RUN;
    onStart();
    return true;
}

bool FSM::pause()
{
    mS = S::PAUSED;
    return true;
}

bool FSM::resume()
{
    mS = S::RUN;
    return true;
}

bool FSM::stop()
{
    exitState(S_ROOT);
    onStop();
    mS = S::IDLE;
    return true;
}

bool FSM::destroy()
{
    onDestroy(*mContext);

    mContext->remove(this);

    for (auto &stateNode : mStateNodeTable)
    {
        FSM* fsm = dynamic_cast<FSM*>(stateNode.stateObject);
        if (!fsm)
        {
            delete stateNode.stateObject;
        }
        else
        {
            fsm->destroy();
        }
    }
    mStateNodeTable.clear();
    mS = S::INVAL;

    release();
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

void FSM::onCreate(const Context& context)
{
    std::string msg;
    msg = std::string("FSM::onCreate(): ") + getName();
    Utils::log(msg);
}

void FSM::onStart()
{
    std::string msg;
    msg = std::string("FSM::onStart(): ") + getName();
    Utils::log(msg);
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

void FSM::onDestroy(const Context& context)
{
    std::string msg;
    msg = std::string("FSM::onDestroy(): ") + getName();
    Utils::log(msg);
}

bool FSM::isInvalid() const
{
    return getS() == S::INVAL;
}

bool FSM::isStateInvalid(sid sID) const
{
    return (sID < 0 || sID >= (int)mStateNodeTable.size()) && sID != S_ROOT;
}

bool FSM::isStateActive(sid sID) const
{
    const StateNode_t& stateNode = getStateNode(sID);
    if (stateNode.stateEntry == mRootNode.stateEntry)
        return  getS() == S::RUN;

    sid parent = stateNode.stateEntry->parent;
    const StateNode_t& parentNode = getStateNode(parent);
    return parentNode.activeChild == sID;
}

sid FSM::getActiveLeafState() const
{
    const StateNode_t* stateNode = &mRootNode;
    while (stateNode->activeChild != S_INVAL)
    {
        stateNode = &getStateNode(stateNode->activeChild);
    }
    return stateNode->stateEntry->id;
}

int FSM::getStateLevel(sid sID) const
{
    int level = 0;
    const StateNode_t* stateNode = &getStateNode(sID);
    sid parent = stateNode->stateEntry->parent;
    while (parent != S_INVAL)
    {
        level++;
        stateNode = &getStateNode(parent);
        parent = stateNode->stateEntry->parent;
    }
    return level;
}

sid FSM::seekParent(sid sID, int level)
{
    if (level < 0)
        return S_INVAL;

    if (isStateInvalid(sID))
        return S_INVAL;

    sid parent = sID;
    const StateNode_t* stateNode = nullptr;
    while (level > 0)
    {
        stateNode = &getStateNode(parent);
        parent = stateNode->stateEntry->parent;
        level--;
    }
    return parent;
}

sid FSM::seekCommonParent(sid sID1, sid sID2)
{
    int level1 = getStateLevel(sID1);
    int level2 = getStateLevel(sID2);
    if (level1 < level2)
        sID2 = seekParent(sID2, level2-level1);
    else if (level1 > level2)
        sID1 = seekParent(sID1, level1-level2);

    while (sID1 != sID2)
    {
        sID1 = seekParent(sID1, 1);
        sID2 = seekParent(sID2, 1);
    }
    return sID1;
}

bool FSM::switchState(sid dstState)
{
    sid activeLeaf = getActiveLeafState();
    if (activeLeaf == dstState)
        return true;

    sid parent = seekCommonParent(activeLeaf, dstState);
    int parentLevel = getStateLevel(parent);

    int deltaLevel = getStateLevel(activeLeaf) - parentLevel;
    if (deltaLevel > 0)
    {
        int srcState = seekParent(activeLeaf, deltaLevel - 1);
        if (!exitState(srcState))
            return false;
    }

    StateNode_t* parentNode = &getStateNode(parent);
    deltaLevel = getStateLevel(dstState) - parentLevel - 1;
    while (deltaLevel > 0)
    {
        LLASSERT(parentNode->activeChild == S_INVAL, "The state should be inactive");

        parentNode->activeChild = seekParent(dstState, deltaLevel);
        StateNode_t* stateNode = &getStateNode(parentNode->activeChild);
        if (stateNode->stateObject && (stateNode->sopFlag & SOP_ENTER) != SOP_ENTER)
        {
            stateNode->sopFlag |= SOP_ENTER;
            stateNode->stateObject->onEnter();
            stateNode->sopFlag &= ~SOP_ENTER;
        }
        parentNode = stateNode;
        deltaLevel--;
    }

    if (!enterState(dstState, false))
        return false;

    return true;
}

int FSM::dispatchEvent(const std::string& evtName, const EvtData& evtData)
{
    return 0;
#if 0
    cntt_uint32 userdata;
    CNTT_RESULT result = CNTT_FSMR_UNTOUCHED;

    if (smachine->smatrix->m_state_stevttree->Find(lpEvtName, userdata) && userdata != 0)
    {
        cntt_list_head *listhead = (cntt_list_head*)userdata;
        TransItem_t *curritem;
        cntt_list_t *curr;

        //any state that is active when the event reaches, the event should be processed. (sometimes the previous event could change the state)
        CNTT_LIST_FOREACH(curr, listhead)
        {
            curritem = CNTT_LIST_ENTRY(curr, TransItem_t, list);
            if (fsm_IsStateActive(smachine->stree, curritem->transdef->curstate))
                curritem->isactive = cntt_true;
            else
                curritem->isactive = cntt_false;
        }

        int level = -1;
        CNTT_LIST_FOREACH(curr, listhead)
        {
            curritem = CNTT_LIST_ENTRY(curr, TransItem_t, list);

            //add offline events in the queue of curstate. only queue once for the transfer items with same level (same curstate)
            if ((curritem->transdef->flag & TFL_OFFLINE) == TFL_OFFLINE && curritem->level != level)
            {
                CNTT_STATENODE_T *statenode = fsm_StateNode(smachine->stree, curritem->transdef->curstate);
                if (!curritem->isactive || cntt_QueueSize(statenode->evtqueue) > 0)
                {
                    if (statenode != NULL && statenode->evtqueue != NULL)
                    {
                        CNTT_QUEUEEVT_T queueevt = { NULL, NULL, 0 };
                        queueevt.evtname = curritem->transdef->evtname;
                        if (pEvtData != NULL && nEvtDataLen > 0)
                        {
                            queueevt.evtdata = (cntt_uint8*) cntt_malloc((cntt_size)nEvtDataLen);
                            if (queueevt.evtdata != NULL)
                            {
                                memcpy(queueevt.evtdata, pEvtData, nEvtDataLen);
                                queueevt.evtdatalen = (cntt_int)nEvtDataLen;
                            }
                        }
                        cntt_QueueEnter(statenode->evtqueue, &queueevt, -1);
                        level = curritem->level;
                    }
                    continue;
                }
            }

            if (!curritem->isactive)
                continue;

            cntt_bool done = cntt_false;
            if ((curritem->transdef->flag & TFL_ENQUEUE) == TFL_ENQUEUE)
            {
                CNTT_STATENODE_T *statenode = fsm_StateNode(smachine->stree, curritem->transdef->nextstate);
                if (statenode != NULL && statenode->evtqueue != NULL)
                {
                    CNTT_QUEUEEVT_T queueevt = { NULL, NULL, 0 };
                    queueevt.evtname = curritem->transdef->evtname;
                    if (pEvtData != NULL && nEvtDataLen > 0)
                    {
                        queueevt.evtdata = (cntt_uint8*) cntt_malloc((cntt_size)nEvtDataLen);
                        if (queueevt.evtdata != NULL)
                        {
                            memcpy(queueevt.evtdata, pEvtData, nEvtDataLen);
                            queueevt.evtdatalen = (cntt_int)nEvtDataLen;
                        }
                    }
                    cntt_QueueEnter(statenode->evtqueue, &queueevt, -1);
                }
                result = CNTT_FSMR_REMOVE;
                done = cntt_true;
            }

            if (!done && (curritem->transdef->flag & TFL_DEQUEUE) == TFL_DEQUEUE)
            {
                CNTT_STATENODE_T *statenode = fsm_StateNode(smachine->stree, curritem->transdef->nextstate);
                if (statenode != NULL && statenode->evtqueue != NULL)
                {
                    CNTT_QUEUEEVT_T queueevt = { NULL, NULL, 0 };
                    if (cntt_QueueConsume(statenode->evtqueue, &queueevt, NULL))
                    {
                        lpEvtName = queueevt.evtname;
                        pEvtData = queueevt.evtdata;
                        nEvtDataLen = queueevt.evtdatalen;
                    }
                    else
                    {
                        done = cntt_true;
                    }
                }
            }

            if (done)
                continue;

            if ((curritem->transdef->flag & TFL_NEXTPROC) == TFL_NEXTPROC)
            {
                if (curritem->transdef->nextstate != CNTT_FSMS_INVAL)
                {
                    if (!fsm_SwitchState(smachine, curritem->transdef->curstate, curritem->transdef->nextstate))
                        return CNTT_FSMR_FATALERR; //if machine has been deleted

                    if (curritem->transdef->onEvtProc)
                    {
                        result = curritem->transdef->onEvtProc(smachine, curritem->transdef->nextstate, lpEvtName, pEvtData, nEvtDataLen, curritem->transdef->userdata);
                        if (!fsm_IsMachine(smachine))
                            return CNTT_FSMR_FATALERR;
                    }
                }
            }
            else
            {
                if (curritem->transdef->onEvtProc)
                {
                    result = curritem->transdef->onEvtProc(smachine, curritem->transdef->curstate, lpEvtName, pEvtData, nEvtDataLen, curritem->transdef->userdata);
                    if (!fsm_IsMachine(smachine))
                        return CNTT_FSMR_FATALERR;
                }

                if (curritem->transdef->nextstate != CNTT_FSMS_INVAL &&
                    (curritem->transdef->flag & TFL_DEQUEUE) != TFL_DEQUEUE) //only read event instead switch state for TFL_DEQUEUE
                {
                    if (!fsm_SwitchState(smachine, curritem->transdef->curstate, curritem->transdef->nextstate))
                        return CNTT_FSMR_FATALERR; //if machine has been deleted
                }
            }

            if ((curritem->transdef->flag & TFL_DEQUEUE) == TFL_DEQUEUE)
            {
                if (pEvtData != NULL)
                    cntt_free(pEvtData);
            }

            if (result != CNTT_FSMR_CONTINUE && result != CNTT_FSMR_UNTOUCHED)
                break;
        }
    }

    return result;
#endif
}

NS_LL_END
