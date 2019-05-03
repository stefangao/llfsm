//============================================================================
// Name        : llFSM.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <algorithm>
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

FSM& FSM::create(const std::string& name, Context& context)
{
    bool ret = createInternal(name, context);
    LLASSERT(ret, "FSM::create failed: %s\n", name.c_str());

    onCreateInternal(context);
    return *this;
}

FSM* FSM::create(FSM* fsm, const std::string& name, Context& context)
{
    fsm->create(name, context);
    return fsm;
}

bool FSM::createInternal(const std::string& name, Context& context)
{
    mContext = &context;
    mName = name;

    mRootNode.stateObject = nullptr;
    mRootNode.stateEntry = &ROOT_ENTRY;
    mRootNode.activeChild = S_INVAL;

    int stateCount = getStateCount();
    const StateEntry_t* stateEntry = getStateTable();
    if (stateCount == 0 && stateEntry != nullptr)
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
        if (stateEntry != nullptr)
        {
            for (int i = 0; i < stateCount; i++)
            {
                LLASSERT(stateEntry->id == i, "State ID is disordered!");
                mStateNodeTable[i].stateEntry = stateEntry;
                mStateNodeTable[i].stateObject = stateEntry->createInstance();
                mStateNodeTable[i].stateObject->mThisFSM = this;
                mStateNodeTable[i].stateObject->mID = stateEntry->id;
                mStateNodeTable[i].stateObject->mStateNode = &mStateNodeTable[i];
                mStateNodeTable[i].hbTimerID = -1;
                stateEntry++;
            }
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
                transEntry++; //skip the header element
                while (transEntry->from != -1)
                {
                    auto& evtName = transEntry->event;
                    auto iter = mEventTransMap.find(evtName);
                    if (iter != mEventTransMap.end())
                    {
                        auto& transVect = iter->second;
                        transVect.push_back(transEntry);
                    }
                    else
                    {
                        std::vector<const TransEntry_t*> transVect;
                        transVect.push_back(transEntry);
                        mEventTransMap[evtName] = transVect;
                    }
                    transEntry++;
                    transCount++;
                }
            }
            builder = builder->__getSuperBuilder();
        } while (builder);

        if (transCount > 0)
        {
            setTransCount(transCount);
        }

        for (auto iter = mEventTransMap.begin(); iter != mEventTransMap.end(); iter++)
        {
            auto& transVect = iter->second;
            std::sort(transVect.begin(), transVect.end(), [this](const TransEntry_t* t1,const TransEntry_t* t2)->bool
                {
                    return getStateLevel(t1->from) > getStateLevel(t2->from);
                });
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

    onCreate(context);
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
        /*
         * 1) start embedded fsm inside the state at first
         * 2) enter the state
         * 3) enter default active state inside the embeded fsm
         */
        FSM* fsm = dynamic_cast<FSM*>(stateNode.stateObject);
        if (fsm)
        {
            if (fsm->mS == S::IDLE)
            {
                fsm->mS = S::RUN;
                fsm->onStart();
            }
            else if (fsm->mS == S::PAUSED)
            {
                fsm->mS = S::RUN;
            }
        }
        stateNode.stateObject->onEnter();
        if (fsm)
        {
            fsm->enterState(S_ROOT, true);
        }
        stateNode.stateObject->processOfflineEvents();
        stateNode.sopFlag &= ~SOP_ENTER;
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
            FSM* fsm = dynamic_cast<FSM*>(stateNode->stateObject);
            if (fsm)
            {
                fsm->exitState(S_ROOT);
            }
            stateNode->stateObject->onExit();
            if (stateNode->hbTimerID != -1)
            {
                killTimer(stateNode->hbTimerID);
                stateNode->hbTimerID = -1;
            }
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
    mS = S::RUN;

    onStart();

    if (!enterState(S_ROOT, true))
        return false;

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
    if (mS == S::RUN)
        stop();

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

bool FSM::sendEvent(const std::string& evtName, const EvtStream& evtData)
{
    if (mS != S::RUN)
        return false;

    dispatchEvent(evtName, evtData);
    return true;
}

bool FSM::postEvent(const std::string& evtName, const EvtStream& evtData)
{
    if (mS != S::RUN)
        return false;

    EvtStream* copyEvtData = nullptr;
    if (!evtData.empty())
    {
        copyEvtData = evtData.clone();
    }

    postCallback([this, evtName, copyEvtData](const void* userData) {
        if (copyEvtData)
        {
            dispatchEvent(evtName, *copyEvtData);
            delete copyEvtData;
        }
        else
        {
            dispatchEvent(evtName, EvtStream::EMPTY);
        }
    });
    return true;
}

void FSM::postBcEvent(const std::string& evtName, const EvtStream& evtData)
{
    mContext->postBcEvent(evtName, evtData);
}

void FSM::sendBcEvent(const std::string& evtName, const EvtStream& evtData)
{
    mContext->sendBcEvent(evtName, evtData);
}
void FSM::onCreate(const Context& context)
{
    LLLOG("FSM::onCreate() name=%s\n", getName().c_str());;
}

void FSM::onStart()
{
    LLLOG("FSM::onStart() name=%s\n", getName().c_str());
}

void FSM::onPause()
{
    LLLOG("FSM::onPause() name=%s\n", getName().c_str());
}

void FSM::onResume()
{
    LLLOG("FSM::onResume() name=%s\n", getName().c_str());
}

void FSM::onStop()
{
    LLLOG("FSM::onStop() name=%s\n", getName().c_str());
}

void FSM::onDestroy(const Context& context)
{
    LLLOG("FSM::onDestroy() name=%s\n", getName().c_str());
}

bool FSM::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    LLLOG("FSM::onEventProc() evtName=%s\n", evtName.c_str());
	return true;
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

bool FSM::changeTo(sid dstState, bool enterDefaultActive)
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
            stateNode->stateObject->processOfflineEvents();
            stateNode->sopFlag &= ~SOP_ENTER;
        }
        parentNode = stateNode;
        deltaLevel--;
    }

    if (!enterState(dstState, enterDefaultActive))
        return false;

    return true;
}

int FSM::dispatchEvent(const std::string& evtName, const EvtStream& evtData)
{
    int result = EVTR_UNTOUCHED;
    std::vector<const TransEntry_t*> activeTransEntries;

    auto iter = mEventTransMap.find(evtName);
    if (iter == mEventTransMap.end())
        return result;

    auto& transVect = iter->second;
    for (auto& entry : transVect)
    {
        sid fromSid = entry->from;
        if (isStateActive(fromSid))
        {
            activeTransEntries.push_back(entry);
        }
        else if ((entry->flag & TFL_OFFLINE) == TFL_OFFLINE)
        {
            auto& stateNode = getStateNode(fromSid);
            auto copyEvtData = evtData.empty() ? &EvtStream::EMPTY : evtData.clone();
            stateNode.offlineEvents.push(std::make_pair(evtName, copyEvtData));
        }
    }

    //call onEventProc of fromState
    for (auto& entry : activeTransEntries)
    {
        sid fromSid = entry->from;
        if ((entry->flag & TFL_TOPROC) != TFL_TOPROC)
        {
            if (fromSid != S_ROOT)
            {
                auto& stateNode = getStateNode(fromSid);
                int r = stateNode.stateObject->onEventProc(evtName, (EvtStream&) evtData);
                if (r)
                    result = EVTR_CONTINUE;
            }
            else
            {
                int r = onEventProc(evtName, (EvtStream&) evtData); //if root state, trigger fsm event proc
                if (r)
                    result = EVTR_CONTINUE;
            }
        }
    }

    //call onEventProc of toState if TFL_TOPROC is marked
    bool stateChanged = false;
    for (auto& entry : activeTransEntries)
    {
        sid fromSid = entry->from;
        if (!stateChanged)
        {
            sid toSid = entry->to;
            if (toSid != S_NONE && toSid != fromSid)
            {
                changeTo(toSid);
            }
            stateChanged = true;
        }

        if ((entry->flag & TFL_TOPROC) == TFL_TOPROC)
        {
            sid toSid = entry->to;
            if (isStateActive(toSid))
            {
                auto& stateNode = getStateNode(toSid);
                int r = stateNode.stateObject->onEventProc(evtName, (EvtStream&)evtData);
                if (r)
                    result = EVTR_CONTINUE;
            }
        }
    }

    return result;
}

void FSM::subscribeBcEvent(const std::string& evtName)
{
    auto iter = mBcEventSubscribeMap.find(evtName);
    if (iter == mBcEventSubscribeMap.end())
    {
        mBcEventSubscribeMap.insert(std::make_pair(evtName, true));
    }
}

void FSM::unsubscribeBcEvent(const std::string& evtName)
{
    auto iter = mBcEventSubscribeMap.find(evtName);
    if (iter != mBcEventSubscribeMap.end())
    {
        mBcEventSubscribeMap.erase(iter);
    }
}

bool FSM::isBcEventSubscribed(const std::string& evtName)
{
    auto iter = mBcEventSubscribeMap.find(evtName);
    return iter != mBcEventSubscribeMap.end();
}

FSM* FSM::getParent() const
{
    return mThisFSM;
}

NS_LL_END
