#include "CellLoadWatcher.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/GameData.h"
#include "Papyrus.h"

CellLoadWatcher *CellLoadWatcher::instance = nullptr;
bool CellLoadWatcher::initRequested = false;
std::string CellLoadWatcher::className = "CellLoadWatcher";

CellLoadWatcher::~CellLoadWatcher() {
}

void CellLoadWatcher::InitHook() {
	new CellLoadWatcher();
	g_cellFullyLoadedEventSource.AddEventSink((BSTEventSink<TESCellFullyLoadedEvent>*)instance);
	_MESSAGE("CellLoadWatcher added to the sink.");
}

void CellLoadWatcher::ResetHook() {
	initRequested = false;
}

void CellLoadWatcher::RequestInit() {
	initRequested = true;
}

bool initialized = false;
EventResult CellLoadWatcher::ReceiveEvent(TESCellFullyLoadedEvent * evn, EventDispatcher<TESCellFullyLoadedEvent>* src) {
	if (!initialized) {
		initialized = true;

		BingleQuickSwitch::KeywordData(static_cast<BGSKeyword*>(LookupFormByID(0x1E711)),
			static_cast<BGSKeyword*>(LookupFormByID(0x1E712)),
			static_cast<BGSKeyword*>(LookupFormByID(0x1E713)),
			static_cast<BGSKeyword*>(LookupFormByID(0x1E714)),
			static_cast<BGSKeyword*>(LookupFormByID(0x6D930)),
			static_cast<BGSKeyword*>(LookupFormByID(0x6D931)),
			static_cast<BGSKeyword*>(LookupFormByID(0x6D932)));

		tArray<TESObjectWEAP*> weapons = DataHandler::GetSingleton()->weapons;
		for (UInt32 i = 0; i < weapons.count; i++) {
			TESObjectWEAP* weapon;
			weapons.GetNthItem(i, weapon);
			BingleQuickSwitch::BaseData(weapon->formID, weapon->equipType.GetEquipSlot(), weapon->gameData.type, BingleQuickSwitch::PullWeaponKeyword(weapon)
				, weapon->gameData.speed, weapon->damage.GetAttackDamage());
			if (weapon->formID == 0x1F4) {
				BingleQuickSwitch::Unarmed = weapon;
			}
		}
	}
	
	return kEvent_Continue;
}
