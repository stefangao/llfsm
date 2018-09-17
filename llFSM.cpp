#include "llFSM.h"
#include <iostream> //TBD

NS_LL_BEGIN

bool FSM::create(const std::string& name, const Context& context)
{
	mContext = &context;

    //statetable init
	const StateEntry_t* stateEntry = getStateTable();
	if (stateEntry == nullptr)
		return false; //no state table is also a reasonable case

	int stateCount = getStateCount();
	if (stateCount == 0)
	{
		while (stateEntry++->id != -1)
			stateCount++;

		LLASSERT(stateCount > 0);

		mStateNodeTable = new StateNode_t[stateCount];

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


#if 0

	m_pStateObjectTable = (CState**)cntt_malloc(sizeof(CState*)*nStateCount);
	if (m_pStateObjectTable == NULL)
		return -1;

	stateEntry = GetStateTable();
	for (int i = 0; i < nStateCount; i++)
	{
		m_pStateObjectTable[i] = stateEntry->object->newInstance();
		m_pStateObjectTable[i]->ThisFSM = this;
		m_pStateObjectTable[i]->m_id = stateEntry->id;
		stateEntry++;
	}

	//transtable init
	auto transInfo = (FSM_STRANSFER_T*)GetTransInfo();
	int nTransCount = GetTransCount();
	if (transInfo == nullptr && nTransCount == 0)
	{
		const CGmbsFSM* builder = this;
		do {
			const TransEntry_t* transEntry = builder->GetTransTable();
			if (transEntry != NULL)
			{
				while (transEntry++->fromstate != -1)
					nTransCount++;
			}
			builder = builder->__getSuperBuilder();
		} while (builder);

		if (nTransCount > 0)
		{
			transInfo = (FSM_STRANSFER_T*) cntt_malloc(sizeof(FSM_STRANSFER_T) * (nTransCount));
			cntt_assert(transInfo != NULL);

			union {
				cntt_void* userdata;
				onEventProc_t eventproc;
			} up = { 0 };

			int i = 0;
			builder = this;
			do {
				const TransEntry_t* transEntry = builder->GetTransTable();
				if (transEntry != NULL)
				{
					while (transEntry->fromstate != -1) {
						transInfo[i].curstate = transEntry->fromstate;
						transInfo[i].evtname = transEntry->eventname;
						transInfo[i].nextstate = transEntry->tostate;
						transInfo[i].onEvtProc = CGmbsFSM::onEventProc;
						transInfo[i].flag = (transEntry->flag | TFL_TEXTEVT);
						up.eventproc = transEntry->eventproc;
						transInfo[i].userdata = up.userdata;
						transInfo[i].isMatch = NULL;
						transEntry++;
						i++;
					}
				}
				builder = builder->__getSuperBuilder();
			} while (builder);
		}
		SetTransInfo(transInfo);
		SetTransCount(nTransCount);
	}



	//assert state table right
	for (i = 0; i < statecount; i++)
	{
		if (statetable[i].id != i)
		{
			cntt_error("error, state id not equal array index");
			return NULL;
		}
	}

	smachine = (CNTT_SMACHINE_T*)cntt_malloc(sizeof(CNTT_SMACHINE_T));
	if (smachine == NULL)
	{
		return NULL;
	}
	cntt_memset(smachine, 0, sizeof(CNTT_SMACHINE_T));
	smachine->curfsmstate = FSM_MACHINE_STOP;
	smachine->modetimer = NULL;
	smachine->minmodeticks = 0xFFFFFFFF;

	stree = (CNTT_STREE_T*)cntt_malloc(sizeof(CNTT_STREE_T));
	if (stree == NULL)
	{
		cntt_free(smachine);
		return NULL;
	}
	cntt_memset(&stree->rootnode, 0, sizeof(CNTT_STATENODE_T));
	INIT_CNTT_LIST_HEAD(&stree->rootnode.childlist);
	stree->rootnode.stateinfo = (CNTT_STATEINFO_T*)cntt_malloc(sizeof(CNTT_STATEINFO_T));
	if (stree->rootnode.stateinfo)
	{
		*stree->rootnode.stateinfo = m_rootstate;
	}

	smatrix = (CNTT_SMATRIX_T*)cntt_malloc(sizeof(CNTT_SMATRIX_T));
	if (smatrix == NULL)
	{
		cntt_free(smachine);
		cntt_free(stree);
		return NULL;
	}
	smatrix->stransfer = transfertable;
	smatrix->stransfersize = transfercount;


	stree->shashsize = statecount;
	stree->shashtable = (CNTT_STATENODE_T**)cntt_malloc(stree->shashsize*sizeof(CNTT_STATENODE_T*));
	if (stree->shashtable == NULL)
	{
		cntt_free(smachine);
		cntt_free(stree);
		cntt_free(smatrix);
		return NULL;
	}

	for (i = 0; i < stree->shashsize; i++)
	{
		statenode = (CNTT_STATENODE_T*)cntt_malloc(sizeof(CNTT_STATENODE_T));
		if (statenode == NULL)
		{
			fsm_DeleteMachine(smachine);
			return NULL;
		}
		cntt_memset(statenode, 0, sizeof(CNTT_STATENODE_T));

		INIT_CNTT_LIST_HEAD(&statenode->childlist);
		statenode->stateinfo = (CNTT_STATEINFO_T*)&statetable[i];
		stree->shashtable[i] = statenode;
		statenode->modemaxticks = CNTT_MODETICK_INFINITE; //disable onMode as default
		statenode->modedelayticks = CNTT_MODETICK_INFINITE;
		statenode->evtqueue = NULL;

		INIT_CNTT_LIST_HEAD(&statenode->timerlist);
	}

	smachine->stree = stree;
	smachine->smatrix = smatrix;

	if (!fsm_MakeStateTree(smachine->stree, CNTT_FSMS_ROOT))
	{
		fsm_DeleteMachine(smachine);
		return NULL;
	}

	fsm_CreateEventTree(smachine);

	smachine->magicnum = FSM_MAGICNUM;

	smachine->resultbuf = (cntt_uint8*)cntt_malloc(CNTT_FSMS_RESULTSIZE);
	smachine->resultbufsize = CNTT_FSMS_RESULTSIZE;
	smachine->resultdatalen = 0;
#endif

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
