#pragma once
#include <vector>

#define ADDR_UnkCellInfo						0x012E32E8
class Actor;
class NiPoint3;
void NormalizeVector(float& x, float& y, float& z);
void NormalizeVector(NiPoint3& vec);

class TargetData {
public:
	TargetData() {
		target = nullptr;
		size = 0;
		dd = 0;
	};
	TargetData(Actor* _target, UInt16 _size, float _dd) {
		target = _target;
		size = _size;
		dd = _dd;
	}
	Actor* target;
	UInt16 size;
	float dd;
};
class ActorModifier {
public:
	static void EnableRestraint(bool b);
	static void RestrainMovement(Actor *a, bool restrain);
	static void RestrainView(Actor *a, bool restrain);
	static void EnableAimHelper(bool b);
	static void LockAim(float aimHelperMinDist, float aimHelperMaxDist);
	static void UnlockAim();
	static void ModifyAV(Actor* a, char *AVname, float v);
	static void SetCurrentAV(Actor* a, char *AVname, float v);
	static float GetAV(Actor * a, char * AVname);
	static float GetAVMax(Actor * a, char * AVname);
};