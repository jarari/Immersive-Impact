#pragma once
#include "SKSE/GameEvents.h"

class EquipWatcher : public BSTEventSink<TESEquipEvent> {
protected:
	static EquipWatcher *instance;
	static bool isInitialized;
public:
	EquipWatcher() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE("Equip hook instance created.");
	}

	static EquipWatcher *GetInstance() {
		return instance;
	}
	virtual ~EquipWatcher();

	static void InitHook();

	static void ResetHook();

	virtual EventResult ReceiveEvent(TESEquipEvent * evn, EventDispatcher<TESEquipEvent>* src) override;
};
