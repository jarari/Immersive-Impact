#pragma once
#include <vector>
class Actor;
class NiMatrix33;
class NiPoint3;
class TESCamera;
class ActiveEffect;
namespace Utils {
	float Scale(NiPoint3 vec);
	void NormalizeVector(float& x, float& y, float& z);
	void NormalizeVector(NiPoint3& vec);
	void GetCameraForward(TESCamera* cam, NiPoint3* vecnorm);
	void SetMatrix33(float a, float b, float c, float d, float e, float f, float g, float h, float i, NiMatrix33& mat);
	NiMatrix33 GetRotationMatrix33(float pitch, float yaw, float roll);
	float Determinant(NiMatrix33 mat);
	NiMatrix33 Inverse(NiMatrix33 mat);
	NiPoint3 WorldToLocal(NiPoint3 wpos, NiPoint3 lorigin, NiMatrix33 rot);
	NiPoint3 LocalToWorld(NiPoint3 lpos, NiPoint3 lorigin, NiMatrix33 rot);
	ActiveEffect* GetActiveEffectFromActor(Actor* actor);
}