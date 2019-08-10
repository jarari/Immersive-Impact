#include "EquipWatcher.h"
#include "MenuCloseWatcher.h"
#include "ConfigHandler.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameReferences.h"
#include "SKSE/GameObjects.h"
#include <SKSE\GameRTTI.h>
#include <SKSE\GameExtraData.h>
#include <SKSE\PapyrusObjectReference.cpp>

EquipWatcher *EquipWatcher::instance = nullptr;
bool EquipWatcher::isInitialized = false;
bool EquipWatcher::isTwoHanded = false;
float EquipWatcher::playerArmorWeight = 0;

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
	isTwoHanded = false;
}

void EquipWatcher::ScanArmorWeight() {
	PlayerCharacter* player = (*g_thePlayer);
	playerArmorWeight = 0.0f;
	if (player->GetNiNode()) {
		TESContainer* container = DYNAMIC_CAST(player->baseForm, TESForm, TESContainer);
		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(player->extraData.GetByType(kExtraData_ContainerChanges));
		tList<ExtraContainerChanges::EntryData> *objList = (tList<ExtraContainerChanges::EntryData>*)pXContainerChanges->data->objList;
		tList<ExtraContainerChanges::EntryData>::Iterator it = objList->Begin();
		while (!it.End()) {
			ExtraContainerChanges::EntryData* extraData = it.Get();
			if (extraData && extraData->type->GetFormType() == FormType::kFormType_Armor) {
				TESForm* form = extraData->type;
				int baseCount = 0;
				if (container)
					baseCount = container->CountItem(form);
				if (extraData->countDelta > 0) {
					ExtraContainerChanges::EquipItemData state;
					extraData->GetEquipItemData(state, form->formID, baseCount);
					if(state.isTypeWorn || state.isTypeWornLeft || state.isItemWorn || state.isItemWornLeft)
						playerArmorWeight += ((TESObjectARMO*)form)->weight.weight;
				}
			}
			++it;
		}
	}
}

EventResult EquipWatcher::ReceiveEvent(TESEquipEvent * evn, EventDispatcher<TESEquipEvent>* src) {
	if (evn->unk_00 == (UInt32)(*g_thePlayer)) {
		if (!((Actor*)(evn->unk_00))->GetEquippedObject(true) && !((Actor*)(evn->unk_00))->GetEquippedObject(false)
			&& !((Actor*)evn->unk_00)->equippingMagicItems[0] && !((Actor*)evn->unk_00)->equippingMagicItems[1])
			MenuCloseWatcher::RequestAction((Actor*)(evn->unk_00));
		if (!isInitialized && (*g_thePlayer)->GetNiNode()) {
			isInitialized = true;
			ConfigHandler::LoadConfig(0);
			TESForm *rweap = ((Actor*)(evn->unk_00))->GetEquippedObject(false);
			TESForm *lweap = ((Actor*)(evn->unk_00))->GetEquippedObject(true);
			if (rweap) {
				ConfigHandler::LoadConfig(rweap->formID, ((TESObjectWEAP*)rweap)->type(), 1);
			}
			if (lweap) {
				ConfigHandler::LoadConfig(lweap->formID, ((TESObjectWEAP*)lweap)->type(), 0);
			}
		}
		if (evn->unk_01) {
			TESForm *equipment = LookupFormByID(evn->unk_01);
			if (equipment && equipment->GetFormType() == FormType::kFormType_Weapon) {
				if (evn->unk_03 & 0x10000) {
					if (((Actor*)(evn->unk_00))->GetEquippedObject(false) == ((Actor*)(evn->unk_00))->GetEquippedObject(false))
						isTwoHanded = true;
					if (((Actor*)(evn->unk_00))->GetEquippedObject(false) == equipment)
						ConfigHandler::LoadConfig(equipment->formID, ((TESObjectWEAP*)equipment)->type(), 1);
					if (((Actor*)(evn->unk_00))->GetEquippedObject(true) == equipment)
						ConfigHandler::LoadConfig(equipment->formID, ((TESObjectWEAP*)equipment)->type(), 0);
				} 
				else {
					if (isTwoHanded)
						isTwoHanded = false;
					if (!((Actor*)(evn->unk_00))->GetEquippedObject(false))
						BingleImmersiveImpact::SetCustomized(1, false);
					if (!((Actor*)(evn->unk_00))->GetEquippedObject(true))
						BingleImmersiveImpact::SetCustomized(0, false);
				}
			}
			else if (equipment && equipment->GetFormType() == FormType::kFormType_Armor) {
				if (evn->unk_03 & 0x10000) {
					playerArmorWeight += ((TESObjectARMO*)equipment)->weight.weight;
				}
				else {
					playerArmorWeight -= ((TESObjectARMO*)equipment)->weight.weight;
				}
				//I'm not gonna track down the total armor weight the whole time, but maybe it should at least check if it's negative or something..
				if (playerArmorWeight < 0)
					ScanArmorWeight();
			}
		}
	}
	return kEvent_Continue;
}
