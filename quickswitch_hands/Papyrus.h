#pragma once
#include <unordered_map>
using std::unordered_map;

class TESObjectWEAP;
class BGSEquipSlot;
class BGSKeyword;
namespace BingleQuickSwitch {
	struct Base {
		BGSEquipSlot *slot;
		UInt8 type;
		BGSKeyword *weapKeyword;
		float speed;
		UInt32 damage;
	};
	static TESObjectWEAP *Unarmed;
	void KeywordData(BGSKeyword *sword, BGSKeyword *waraxe, BGSKeyword *dagger, BGSKeyword *mace, BGSKeyword *warhammer, BGSKeyword *greatsword, BGSKeyword *battleaxe);
	BGSKeyword *PullWeaponKeyword(TESObjectWEAP *wep);
	void BaseData(UInt32 formid, BGSEquipSlot *slot, UInt8 type, BGSKeyword *weapKeyword, float speed, UInt32 damage);
}

//Register papyrus functions
class VMClassRegistry;
namespace Papyrus {
	bool RegisterFuncs(VMClassRegistry* registry);
};