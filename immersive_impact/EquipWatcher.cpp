#include "EquipWatcher.h"
#include "MenuCloseWatcher.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameReferences.h"

EquipWatcher *EquipWatcher::instance = nullptr;

bool EquipWatcher::isInitialized = false;

EquipWatcher::~EquipWatcher() {
}

void EquipWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new EquipWatcher();
	g_equipEventSource.AddEventSink((BSTEventSink<TESEquipEvent>*)instance);
	_MESSAGE("Equip hook added to the sink.");
}

void EquipWatcher::ResetHook() {
	isInitialized = false;
}

EventResult EquipWatcher::ReceiveEvent(TESEquipEvent * evn, EventDispatcher<TESEquipEvent>* src) {
	if (evn->unk_00 == (UInt32)(*g_thePlayer)) {
		if (!papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 0) && !papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 1)
			&& !((Actor*)evn->unk_00)->equippingMagicItems[0] && !((Actor*)evn->unk_00)->equippingMagicItems[1])
			MenuCloseWatcher::RequestAction((Actor*)(evn->unk_00));
		if (!isInitialized) {
			isInitialized = true;
			BingleEventInvoker::InitializeRequest();
		}
	}
	return kEvent_Continue;
}
