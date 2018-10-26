#include "llFSM.h"
#include <iostream> //TBD

NS_LL_BEGIN

bool FSM::create(const std::string& name, const Context& context)
{
	mContext = &context;

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
			mStateNodeTable[i].stateEntry = stateEntry;
			mStateNodeTable[i].stateObject = stateEntry->object->newInstance();
			mStateNodeTable[i].stateObject->mThisFSM = this;
			mStateNodeTable[i].stateObject->mID = stateEntry->id;
			stateEntry++;
		}
		setStateCount(stateCount);
	}

	int transCount = getTransCount();
	if (transCount == 0)
	{
		const FSM* builder = this;
		do {
			const TransEntry_t* transEntry = builder->getTransTable();
			if (transEntry != NULL)
			{
				while (transEntry++->fromstate != -1)
					transCount++;
			}
			builder = builder->__getSuperBuilder();
		} while (builder);

		if (transCount > 0)
		{

			setTransCount(transCount);
		}
	}

	return true;
}

StateNode_t& FSM::getStateNode(sid sID)
{
    if (sID == FSMS_ROOT)
        return mRootNode;

    LLASSERT(sID >= 0 && sID < mStateNodeTable.size(), "error");

    return mStateNodeTable[sID];
}

bool FSM::buildStateTree(sid parent)
{
    StateNode_t& parentNode = getStateNode(parent);
    parentNode.activeChild = FSMS_INVAL;
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

bool FSM::start()
{
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

void FSM::printX()
{
    const FSM* fsm = this;
    do
    {
        std::cout << fsm->getX() << std::endl;
        fsm = fsm->__getSuperBuilder();
    } while (fsm);
}

NS_LL_END
