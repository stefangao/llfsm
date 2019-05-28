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
const StateEntry_t FSM::ROOT_ENTRY = { S_ROOT, nullptr, S_INVAL, SFL_ZERO, "FSM::ROOT" };

FSM::FSM()
{
    mContext = nullptr;
    mS = S::INVAL;
    mName = LL_STR_EMPTY;
}

FSM::~FSM()
{
    for (auto iter = mEventTransMap.begin(); iter != mEventTransMap.end(); iter++)
    {
        auto& transVect = iter->second;
        for (int i = 0; i < (int)transVect.size(); i++)
        {
            auto transEntry = transVect[i];
            if ((transEntry->flag & TFL_DYNAMIC) == TFL_DYNAMIC)
            {
                delete transEntry;
            }
        }
    }
}

FSM& FSM::create(const std::string& name, Context& context, void* params)
{
    bool ret = createInternal(name, context);
    LLASSERT(ret, "FSM::create failed: %s\n", name.c_str());

    onCreateInternal(params);
    return *this;
}

FSM* FSM::create(FSM* fsm, const std::string& name, Context& context, void* params)
{
    fsm->create(name, context, params);
    return fsm;
}

bool FSM::createInternal(const std::string& name, Context& context)
{
    mContext = &context;
    mName = name;

    mRootNode.stateObject = this; //FSM is also a state
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
            std::sort(transVect.begin(), transVect.end(), [this](const TransEntry_t* t1, const TransEntry_t* t2)->bool
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

void FSM::onCreateInternal(void* params)
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
            fsm->onCreateInternal(params);
        }
    }

    onCreate(params);
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
    for (int i = 0; i < (int)mStateNodeTable.size(); i++)
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

bool FSM::enterDefaultActiveState()
{
    LLASSERT(mRootNode.activeChild == S_INVAL, "Root state has been activated");

    if (mRootNode.activeChild == S_INVAL)
    {
        sid activeChild = S_INVAL;
        for (auto childNode : mRootNode.childNodes)
        {
            if (childNode->stateEntry->flag & SFL_ACTIVE)
            {
                activeChild = childNode->stateEntry->id;
                break;
            }
        }
        if (activeChild != S_INVAL)
        {
            return enterState(activeChild, true);
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
        LLASSERT(parentNode.activeChild == S_INVAL, "the state must be inactive");

        if (parentNode.activeChild == sID && !enterDefaultActive)
            return true;

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
            fsm->enterDefaultActiveState();
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
    LLASSERT(mS == S::IDLE, "FSM state must be IDLE for start.");

    mS = S::RUN;

    onStart();

    if (!enterDefaultActiveState())
        return false;

    return true;
}

bool FSM::pause()
{
    LLASSERT(mS == S::RUN, "FSM state must be RUN for pause.");

    mS = S::PAUSED;
    return true;
}

bool FSM::resume()
{
    LLASSERT(mS == S::PAUSED, "FSM state must be PAUSED for resume.");

    mS = S::RUN;
    return true;
}

bool FSM::stop()
{
    LLASSERT(mS != S::IDLE, "FSM state must not be IDLE for stop.");

    exitState(S_ROOT);
    onStop();
    mS = S::IDLE;
    return true;
}

bool FSM::destroy()
{
    if (mContext)
    {
        if (mS == S::RUN)
            stop();

        onDestroy();

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
        mContext = nullptr;
    }
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

bool FSM::sendRequest(const std::string& evtName, const EvtStream& reqData, EvtStream& rspData)
{
    if (mS != S::RUN)
        return false;

    dispatchEvent(evtName, reqData, true, rspData);
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

void FSM::delayPostEvent(int delayTime, const std::string& evtName, const EvtStream& evtData)
{
    EvtStream* copyEvtData = nullptr;
    if (!evtData.empty())
    {
        copyEvtData = evtData.clone();
    }

    setTimer(delayTime, [this, evtName, copyEvtData](int tid, const void* userData) {
        killTimer(tid);
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
}

void FSM::postBcEvent(const std::string& evtName, const EvtStream& evtData)
{
    mContext->postBcEvent(evtName, evtData);
}

void FSM::sendBcEvent(const std::string& evtName, const EvtStream& evtData)
{
    mContext->sendBcEvent(evtName, evtData);
}
void FSM::onCreate(void* params)
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

void FSM::onDestroy()
{
    LLLOG("FSM::onDestroy() name=%s\n", getName().c_str());
}

bool FSM::onEventProc(const std::string& evtName, EvtStream& evtData)
{
    LLLOG("FSM::onEventProc() evtName=%s\n", evtName.c_str());
    return true;
}

bool FSM::onRequestProc(const std::string& evtName, EvtStream& evtData, EvtStream& rspData)
{
    LLLOG("FSM::onRequestProc() evtName=%s\n", evtName.c_str());
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
        sID2 = seekParent(sID2, level2 - level1);
    else if (level1 > level2)
        sID1 = seekParent(sID1, level1 - level2);

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

int FSM::executeStateEventHandler(sid sID, const std::string& evtName, const EvtStream& evtData, bool isRequest, EvtStream& rspData)
{
    bool ret = false;
    auto stateObject = getStateNode(sID).stateObject;
    if (stateObject)
    {
        if (!isRequest)
        {
            auto iter = stateObject->mEventHandlerMap.find(evtName);
            if (iter != stateObject->mEventHandlerMap.end())
            {
                auto& eventHandler = iter->second;
                ret = eventHandler((EvtStream&)evtData);
            }
            else
            {
                ret = stateObject->onEventProc(evtName, (EvtStream&)evtData);
            }
        }
        else
        {
            auto iter = stateObject->mRequestHandlerMap.find(evtName);
            if (iter != stateObject->mRequestHandlerMap.end())
            {
                auto& requestHandler = iter->second;
                ret = requestHandler((EvtStream&)evtData, rspData);
            }
            else
            {
                ret = stateObject->onRequestProc(evtName, (EvtStream&)evtData, rspData);
            }
        }
    }

    return ret;
}

int FSM::dispatchEvent(const std::string& evtName, const EvtStream& evtData, bool isRequest, EvtStream& rspData)
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
            if (executeStateEventHandler(fromSid, evtName, evtData, isRequest, rspData))
                result = EVTR_CONTINUE;
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
                if (executeStateEventHandler(toSid, evtName, evtData, isRequest, rspData))
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

void FSM::inteceptEventHandler(const std::string& evtName, const EventHandler& handler, sid sID)
{
    auto& stateNode = getStateNode(sID);
    stateNode.stateObject->addEventHandler(evtName, handler);

    auto iter = mEventTransMap.find(evtName);
    if (iter == mEventTransMap.end())
    {
        auto transEntry = new TransEntry_t(sID, evtName.c_str(), S_NONE, TFL_DYNAMIC);
        std::vector<const TransEntry_t*> transVect;
        transVect.push_back(transEntry);
        mEventTransMap[evtName] = transVect;
    }
}

void FSM::restoreEventHandler(const std::string& evtName, sid sID)
{
    auto& stateNode = getStateNode(sID);
    stateNode.stateObject->removeEventHandler(evtName);
}

void FSM::inteceptRequestHandler(const std::string& evtName, const RequestHandler& handler, sid sID)
{
    auto& stateNode = getStateNode(sID);
    stateNode.stateObject->addRequestHandler(evtName, handler);

    auto iter = mEventTransMap.find(evtName);
    if (iter == mEventTransMap.end())
    {
        auto transEntry = new TransEntry_t(sID, evtName.c_str(), S_NONE, TFL_DYNAMIC);
        std::vector<const TransEntry_t*> transVect;
        transVect.push_back(transEntry);
        mEventTransMap[evtName] = transVect;
    }
}

void FSM::restoreRequestHandler(const std::string& evtName, sid sID)
{
    auto& stateNode = getStateNode(sID);
    stateNode.stateObject->removeRequestHandler(evtName);
}

NS_LL_END
