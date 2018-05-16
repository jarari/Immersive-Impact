#include "Papyrus.h"
#include "SKSE/PapyrusNativeFunctions.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/PapyrusWeapon.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameData.h"
#include <chrono>
#include <thread>
#include <future>

namespace BingleQuickSwitch {
	unordered_map<UInt32, Base> basemap;
	BGSKeyword *WeapTypeSword;
	BGSKeyword *WeapTypeWarAxe;
	BGSKeyword *WeapTypeDagger;
	BGSKeyword *WeapTypeMace;
	BGSKeyword *WeapTypeWarhammer;
	BGSKeyword *WeapTypeGreatsword;
	BGSKeyword *WeapTypeBattleaxe;
	enum Multiplier {
		Damage_1Hto2H,
		Damage_2Hto1H,
		Speed_1Hto2H,
		Speed_2Hto1H
	};
	float multipliers[4];

	void SwapKeyword(TESObjectWEAP *wep, BGSKeyword *swapto) {
		for (int i = 0; i < wep->keyword.numKeywords; i++) {
			if (wep->keyword.keywords[i] == swapto)
				break;
			if (wep->keyword.keywords[i] == WeapTypeSword || wep->keyword.keywords[i] == WeapTypeWarAxe
				|| wep->keyword.keywords[i] == WeapTypeDagger || wep->keyword.keywords[i] == WeapTypeMace
				|| wep->keyword.keywords[i] == WeapTypeWarhammer || wep->keyword.keywords[i] == WeapTypeGreatsword
				|| wep->keyword.keywords[i] == WeapTypeBattleaxe) {
				_MESSAGE("Keyword found %lu. Swapping to %lu", wep->keyword.keywords[i]->formID, swapto->formID);
				wep->keyword.keywords[i] = swapto;
				break;
			}
		}
	}

	void Set1H(TESObjectWEAP *wep, UInt32 reEquip = 1) {
		_MESSAGE("%s, type %u", wep->fullName.name.data, basemap[wep->formID].type);
		if (basemap[wep->formID].type >= 1 && basemap[wep->formID].type <= 4) {
			papyrusWeapon::SetEquipType(wep, basemap[wep->formID].slot);
			papyrusWeapon::SetWeaponType(wep, basemap[wep->formID].type);
			SwapKeyword(wep, basemap[wep->formID].weapKeyword);
			papyrusWeapon::SetSpeed(wep, basemap[wep->formID].speed);
			papyrusWeapon::SetBaseDamage(wep, basemap[wep->formID].damage);
		} else {
			papyrusWeapon::SetEquipType(wep, (BGSEquipSlot *)LookupFormByID(0x00013F44));
			if (basemap[wep->formID].type == 5) {
				_MESSAGE("To Sword");
				papyrusWeapon::SetWeaponType(wep, 1);
				SwapKeyword(wep, WeapTypeSword);
			}
			else if (basemap[wep->formID].type == 6) {
				_MESSAGE("To WarAxe");
				papyrusWeapon::SetWeaponType(wep, 3);
				SwapKeyword(wep, WeapTypeWarAxe);
			}
			papyrusWeapon::SetSpeed(wep, basemap[wep->formID].speed * multipliers[Speed_2Hto1H]);
			papyrusWeapon::SetBaseDamage(wep, basemap[wep->formID].damage * multipliers[Damage_2Hto1H]);
		}
		wep->gameData.skill = 6;
		Actor *p = (Actor*)(*g_thePlayer);
		if (reEquip) {
			_MESSAGE("Request equip");
			//papyrusActor::EquipItemEx(p, wep, 0, false, false);
			BingleEventInvoker::EquipItem(p, wep, 0);
		}
		
	}

	void Set2H(TESObjectWEAP *wep, UInt32 reEquip = 1) {
		_MESSAGE("%s, type %u", wep->fullName.name.data, basemap[wep->formID].type);
		if (basemap[wep->formID].type >= 5 && basemap[wep->formID].type <= 6) {
			papyrusWeapon::SetEquipType(wep, basemap[wep->formID].slot);
			papyrusWeapon::SetWeaponType(wep, basemap[wep->formID].type);
			SwapKeyword(wep, basemap[wep->formID].weapKeyword);
			papyrusWeapon::SetSpeed(wep, basemap[wep->formID].speed);
			papyrusWeapon::SetBaseDamage(wep, basemap[wep->formID].damage);
		} else {
			papyrusWeapon::SetEquipType(wep, (BGSEquipSlot *)LookupFormByID(0x00013F45));
			papyrusWeapon::SetWeaponType(wep, floor((basemap[wep->formID].type - 1) / 2) + 5);
			switch (basemap[wep->formID].type) {
			case 1:
				_MESSAGE("To Greatsword");
				SwapKeyword(wep, WeapTypeGreatsword);
				break;
			case 2:
				_MESSAGE("To Greatsword");
				SwapKeyword(wep, WeapTypeGreatsword);
				break;
			case 3:
				_MESSAGE("To Battleaxe");
				SwapKeyword(wep, WeapTypeBattleaxe);
				break;
			case 4:
				_MESSAGE("To Battleaxe");
				SwapKeyword(wep, WeapTypeBattleaxe);
				break;
			}
			papyrusWeapon::SetSpeed(wep, basemap[wep->formID].speed * multipliers[Speed_1Hto2H]);
			papyrusWeapon::SetBaseDamage(wep, basemap[wep->formID].damage * multipliers[Damage_1Hto2H]);
		}
		wep->gameData.skill = 7;
		Actor *p = (Actor*)(*g_thePlayer);
		if (reEquip) {
			_MESSAGE("Request equip");
			//papyrusActor::EquipItemEx(p, wep, 0, false, false);
			BingleEventInvoker::EquipItem(p, wep, 1);
		}
	}
	
	void RevertHands(StaticFunctionTag *base, TESObjectWEAP *wep) {
		if (wep->type() != basemap[wep->formID].type) {
			papyrusWeapon::SetWeaponType(wep, basemap[wep->formID].type);
		}
	}

	void SwitchHands(StaticFunctionTag *base, TESObjectWEAP *wep, UInt32 _2h = -1, UInt32 reEquip = 1) {
		Actor *p = (Actor*)(*g_thePlayer);

		if (reEquip) {
			papyrusActor::EquipItemEx(p, Unarmed, 1, false, false);
			papyrusActor::EquipItemEx(p, Unarmed, 2, false, false);
		}
		
		if (_2h == -1) {
			if (wep->type() == 5 || wep->type() == 6) {
				Set1H(wep, reEquip);
			} else {
				if (wep->type() != 5 && wep->type() != 6)
					Set2H(wep, reEquip);
			}
		} else {
			if (_2h == 0) {
				Set1H(wep, reEquip);
			} else {
				Set2H(wep, reEquip);
			}
		}
	}

	UInt32 SwitchHandsShort(StaticFunctionTag *base, TESObjectWEAP *wep) {
		Actor *p = (Actor*)(*g_thePlayer);
		TESObjectWEAP *_wep = wep;
		if (!wep)
			_wep = (TESObjectWEAP*)papyrusActor::GetEquippedObject(p, 1);
		if (!_wep || _wep->type() == 0 || _wep->type() >= 7)
			return 0;
		
		TESObjectWEAP *left = (TESObjectWEAP*)papyrusActor::GetEquippedObject(p, 0);
		TESObjectWEAP *right = (TESObjectWEAP*)papyrusActor::GetEquippedObject(p, 1);
		if ((right && !left) || (right && right->equipType.GetEquipSlot() == (BGSEquipSlot *)LookupFormByID(0x00013F45))) {
			bool isStaggering, isEquipping, isUnequipping, bInJumpState;
			p->animGraphHolder.GetAnimationVariableBool("isStaggering", isStaggering);
			p->animGraphHolder.GetAnimationVariableBool("isEquipping", isEquipping);
			p->animGraphHolder.GetAnimationVariableBool("isUnequipping", isUnequipping);
			p->animGraphHolder.GetAnimationVariableBool("bInJumpState", bInJumpState);
			if (!isStaggering && !isEquipping && !isUnequipping && !bInJumpState) {
				papyrusActor::SheatheWeapon(p);
				papyrusActor::UnequipItemEx(p, wep, 1, true);
				papyrusActor::UnequipItemEx(p, wep, 2, true);
				SwitchHands(base, _wep);
				return 1;
			}
		}
		return 0;
	}

	void KeywordData(BGSKeyword * sword, BGSKeyword * waraxe, BGSKeyword * dagger, BGSKeyword * mace, BGSKeyword * warhammer, BGSKeyword * greatsword, BGSKeyword * battleaxe) {
		WeapTypeSword = sword;
		WeapTypeWarAxe = waraxe;
		WeapTypeDagger = dagger;
		WeapTypeMace = mace;
		WeapTypeWarhammer = warhammer;
		WeapTypeGreatsword = greatsword;
		WeapTypeBattleaxe = battleaxe;
	}

	BGSKeyword * PullWeaponKeyword(TESObjectWEAP * wep) {
		for (int i = 0; i < wep->keyword.numKeywords; i++) {
			if (wep->keyword.keywords[i] == WeapTypeSword || wep->keyword.keywords[i] == WeapTypeWarAxe
				|| wep->keyword.keywords[i] == WeapTypeDagger || wep->keyword.keywords[i] == WeapTypeMace
				|| wep->keyword.keywords[i] == WeapTypeWarhammer || wep->keyword.keywords[i] == WeapTypeGreatsword
				|| wep->keyword.keywords[i] == WeapTypeBattleaxe) {
				return wep->keyword.keywords[i];
			}
		}
		return nullptr;
	}

	void BaseData(UInt32 formid, BGSEquipSlot * slot, UInt8 type, BGSKeyword *weapKeyword, float speed, UInt32 damage) {
		Base b;
		b.slot = slot;
		b.type = type;
		b.weapKeyword = weapKeyword;
		b.speed = speed;
		b.damage = damage;
		basemap[formid] = b;
	}

	void RegisterForEquipRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for equip request");
		g_equipRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void UnregisterForEquipRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		g_equipRequestRegs.Unregister<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void UpdateMultiplier(StaticFunctionTag *base, UInt32 type, float val) {
		multipliers[type] = val;
	}
}


bool Papyrus::RegisterFuncs(VMClassRegistry * registry) {
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, TESObjectWEAP*>("SwitchHandsShort", "BingleQuickSwitch", BingleQuickSwitch::SwitchHandsShort, registry));
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, TESObjectWEAP*, UInt32, UInt32>("SwitchHands", "BingleQuickSwitch", BingleQuickSwitch::SwitchHands, registry));
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, TESObjectWEAP*>("RevertHands", "BingleQuickSwitch", BingleQuickSwitch::RevertHands, registry));
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForEquipRequest", "BingleQuickSwitch", BingleQuickSwitch::RegisterForEquipRequest, registry));
	registry->SetFunctionFlags("BingleQuickSwitch", "RegisterForEquipRequest", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("UnregisterForEquipRequest", "BingleQuickSwitch", BingleQuickSwitch::UnregisterForEquipRequest, registry));
	registry->SetFunctionFlags("BingleQuickSwitch", "UnregisterForEquipRequest", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, UInt32, float>("UpdateMultiplier", "BingleQuickSwitchMCM", BingleQuickSwitch::UpdateMultiplier, registry));
	return true;
}
