#pragma once
#include <vector>

#define ADDR_UnkCellInfo						0x012E32E8
class Actor;
class TargetData {
public:
	TargetData() {
		target = nullptr;
		size = 0;
	};
	TargetData(Actor* _target, float _size) {
		target = _target;
		size = _size;
	}
	Actor* target;
	float size;
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