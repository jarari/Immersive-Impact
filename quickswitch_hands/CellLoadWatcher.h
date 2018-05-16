#pragma once
#include "SKSE/GameEvents.h"

class CellLoadWatcher : public BSTEventSink<TESCellFullyLoadedEvent> {
protected:
	static CellLoadWatcher *instance;
	static bool initRequested;
	static std::string className;
public:
	CellLoadWatcher() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE((className + std::string(" instance created.")).c_str());
	}

	virtual ~CellLoadWatcher();

	static void InitHook();

	static void ResetHook();

	static void RequestInit();

	virtual EventResult ReceiveEvent(TESCellFullyLoadedEvent * evn, EventDispatcher<TESCellFullyLoadedEvent>* src) override;
};
