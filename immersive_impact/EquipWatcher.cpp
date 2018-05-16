#include "EquipWatcher.h"
#include "MenuCloseWatcher.h"
#include "ConfigHandler.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameReferences.h"
#include "SKSE/GameObjects.h"

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
		if (!isInitialized && (*g_thePlayer)->GetNiNode()) {
			isInitialized = true;
			ConfigHandler::LoadConfig("General");
			TESForm *rweap = papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 1);
			TESForm *lweap = papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 0);
			if (rweap) {
				ConfigHandler::LoadConfig(((TESObjectWEAP*)rweap)->fullName.name.data, ((TESObjectWEAP*)rweap)->type(), 1);
			}
				
			if (lweap) {
				ConfigHandler::LoadConfig(((TESObjectWEAP*)lweap)->fullName.name.data, ((TESObjectWEAP*)lweap)->type(), 0);
			}
			BingleEventInvoker::InitializeRequest();
		}
		if (evn->unk_01) {
			TESForm *equipment = LookupFormByID(evn->unk_01);
			if (equipment && equipment->GetFormType() == FormType::kFormType_Weapon) {
				if (evn->unk_03 & 0x10000) {
					if (papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 1) == equipment)
						ConfigHandler::LoadConfig(((TESObjectWEAP*)equipment)->fullName.name.data, ((TESObjectWEAP*)equipment)->type(), 1);
					if (papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 0) == equipment)
						ConfigHandler::LoadConfig(((TESObjectWEAP*)equipment)->fullName.name.data, ((TESObjectWEAP*)equipment)->type(), 0);
				} else {
					if (!papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 1))
						BingleImmersiveImpact::SetCustomized(1, false);
					if (!papyrusActor::GetEquippedObject((Actor*)evn->unk_00, 0))
						BingleImmersiveImpact::SetCustomized(0, false);
				}
			}
		}
	}
	return kEvent_Continue;
}
