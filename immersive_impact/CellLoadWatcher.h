#pragma once
#include "SKSE/GameEvents.h"

class CellLoadWatcher : public BSTEventSink<TESCellFullyLoadedEvent> {
protected:
	static CellLoadWatcher *instance;
public:
	virtual ~CellLoadWatcher();

	static void InitHook();

	virtual EventResult ReceiveEvent(TESCellFullyLoadedEvent * evn, EventDispatcher<TESCellFullyLoadedEvent>* src) override;
};
