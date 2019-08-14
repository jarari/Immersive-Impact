#pragma once
#include "SKSE/GameEvents.h"

class TESAmmo;
class EquipWatcher : public BSTEventSink<TESEquipEvent> {
protected:
	static EquipWatcher *instance;
public:
	EquipWatcher() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE("Equip hook instance created.");
	}
	static bool isInitialized;
	static bool isTwoHanded;
	static TESAmmo* equippedAmmo;

	static EquipWatcher *GetInstance() {
		return instance;
	}
	virtual ~EquipWatcher();

	static void InitHook();

	static void ResetHook();

	static void OnFirstLoad();

	static void ScanEquippedItems();

	static float playerArmorWeight;

	virtual EventResult ReceiveEvent(TESEquipEvent * evn, EventDispatcher<TESEquipEvent>* src) override;
};
