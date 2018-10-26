#include "llState.h"
#include "llFSM.h"

NS_LL_BEGIN

State::State()
{
    mID = -1;
    mThisFSM = nullptr;
}

bool State::sendEvent(const std::string& evtName, const EvtData& evtData)
{
    return mThisFSM->sendEvent(evtName, evtData);
}

bool State::postEvent(const std::string& evtName, const EvtData& evtData)
{
    return mThisFSM->postEvent(evtName, evtData);
}

NS_LL_END
