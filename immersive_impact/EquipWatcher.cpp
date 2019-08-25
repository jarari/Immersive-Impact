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
#include "HitFeedback.h"
#include <SKSE/PapyrusActor.h>
#include <immersive_impact\BingleEventInvoker.h>

EquipWatcher *EquipWatcher::instance = nullptr;
bool EquipWatcher::isInitialized = false;
bool EquipWatcher::isTwoHanded = false;
float EquipWatcher::playerArmorWeight = 0;
TESAmmo *EquipWatcher::equippedAmmo = nullptr;

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
	_MESSAGE("EquipWatcher reset");
	isInitialized = false;
	isTwoHanded = false;
	MenuManager* mm = MenuManager::GetSingleton();
	UIManager* ui = UIManager::GetSingleton();
	if(mm && !mm->IsMenuOpen(&BSFixedString("HitFeedbackHelper")) && ui)
		CALL_MEMBER_FN(ui, AddMessage)(&StringCache::Ref("HitFeedbackHelper"), UIMessage::kMessage_Open, nullptr);
}

void EquipWatcher::OnFirstLoad() {
	if (!((Actor*)(*g_thePlayer))->GetNiNode())
		return;
	isInitialized = true;
	ConfigHandler::LoadConfig(0);
	TESForm* rweap = ((Actor*)(*g_thePlayer))->GetEquippedObject(false);
	TESForm* lweap = ((Actor*)(*g_thePlayer))->GetEquippedObject(true);
	if (rweap) {
		ConfigHandler::LoadConfig(rweap->formID, ((TESObjectWEAP*)rweap)->type(), 1);
	}
	if (lweap) {
		ConfigHandler::LoadConfig(lweap->formID, ((TESObjectWEAP*)lweap)->type(), 0);
	}
	ScanEquippedItems();
	HitFeedback::ResetHook();
}

void EquipWatcher::ScanEquippedItems() {
	PlayerCharacter* player = (*g_thePlayer);
	playerArmorWeight = 0.0f;
	if (player->GetNiNode()) {
		TESContainer* container = DYNAMIC_CAST(player->baseForm, TESForm, TESContainer);
		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(player->extraData.GetByType(kExtraData_ContainerChanges));
		tList<ExtraContainerChanges::EntryData> *objList = (tList<ExtraContainerChanges::EntryData>*)pXContainerChanges->data->objList;
		tList<ExtraContainerChanges::EntryData>::Iterator it = objList->Begin();
		while (!it.End()) {
			ExtraContainerChanges::EntryData* extraData = it.Get();
			if (extraData){
				TESForm* form = extraData->type;
				int baseCount = 0;
				if (container)
					baseCount = container->CountItem(form);
				if (extraData->countDelta > 0) {
					ExtraContainerChanges::EquipItemData state;
					extraData->GetEquipItemData(state, form->formID, baseCount);
					if (state.isTypeWorn || state.isTypeWornLeft || state.isItemWorn || state.isItemWornLeft)
						if (extraData->type->GetFormType() == FormType::kFormType_Armor)
							playerArmorWeight += ((TESObjectARMO*)form)->weight.weight;
						else if (extraData->type->GetFormType() == FormType::kFormType_Ammo)
							equippedAmmo = DYNAMIC_CAST(extraData->type, TESForm, TESAmmo);
				}
			}
			++it;
		}
	}
	if (!player->GetEquippedObject(true) && !player->GetEquippedObject(false)
		&& !player->equippingMagicItems[0] && !player->equippingMagicItems[1]) {
		BingleEventInvoker::EquipFist(player);
	}
}

EventResult EquipWatcher::ReceiveEvent(TESEquipEvent * evn, EventDispatcher<TESEquipEvent>* src) {
	if (evn->unk_00 == (UInt32)(*g_thePlayer)) {
		if (!isInitialized && (*g_thePlayer)->GetNiNode()) {
			OnFirstLoad();
		}
		if (evn->unk_01) {
			TESForm *equipment = LookupFormByID(evn->unk_01);
			if (equipment && equipment->formID == 0x1F4)
				return kEvent_Continue;

			if (!(evn->unk_03 & 0x10000)
				&& !((Actor*)(evn->unk_00))->GetEquippedObject(true) && !((Actor*)(evn->unk_00))->GetEquippedObject(false)
				&& !((Actor*)evn->unk_00)->equippingMagicItems[0] && !((Actor*)evn->unk_00)->equippingMagicItems[1]) {
				//MenuCloseWatcher::RequestAction((Actor*)(evn->unk_00));
				BingleEventInvoker::EquipFist((Actor*)evn->unk_00);
			}
			
			if (equipment && equipment->GetFormType() == FormType::kFormType_Weapon) {
				if (evn->unk_03 & 0x10000) {
					if (((Actor*)(evn->unk_00))->GetEquippedObject(false) == ((Actor*)(evn->unk_00))->GetEquippedObject(true))
						isTwoHanded = true;
					else
						isTwoHanded = false;
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
					ScanEquippedItems();
			}
			else if (equipment && equipment->GetFormType() == FormType::kFormType_Ammo) {
				if (evn->unk_03 & 0x10000) {
					equippedAmmo = DYNAMIC_CAST(equipment, TESForm, TESAmmo);
				}
				else {
					equippedAmmo = nullptr;
				}
			}
		}
	}
	return kEvent_Continue;
}
