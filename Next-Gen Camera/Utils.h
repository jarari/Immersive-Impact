#pragma once
#include <vector>
class Actor;
class NiMatrix33;
class NiPoint3;
class TESCamera;
class ActiveEffect;
class TESObjectREFR;
struct Quaternion {
public:
	float x, y, z, w;
	Quaternion(float _x, float _y, float _z, float _w);
	float Norm();
	NiMatrix33 ToRotationMatrix33();
};
namespace Utils {
	float Scale(NiPoint3 vec);
	void NormalizeVector(float& x, float& y, float& z);
	void NormalizeVector(NiPoint3& vec);
	void GetCameraForward(TESCamera* cam, NiPoint3& vecnorm);
	void SetMatrix33(float a, float b, float c, float d, float e, float f, float g, float h, float i, NiMatrix33& mat);
	NiMatrix33 GetRotationMatrix33(float pitch, float yaw, float roll);
	NiMatrix33 GetRotationMatrix33(NiPoint3 axis, float angle);
	float Determinant(NiMatrix33 mat);
	NiMatrix33 Inverse(NiMatrix33 mat);
	NiPoint3 WorldToLocal(NiPoint3 wpos, NiPoint3 lorigin, NiMatrix33 rot);
	NiPoint3 LocalToWorld(NiPoint3 lpos, NiPoint3 lorigin, NiMatrix33 rot);
	NiPoint3 Lerp(NiPoint3 start, NiPoint3 end, float t);
	void VecToAng(NiPoint3 vec, float& pitch, float& yaw);
	bool GetNodePosition(TESObjectREFR* ref, const char* nodeName, NiPoint3& point);
	bool GetHeadPos(TESObjectREFR* ref, NiPoint3& point);
	ActiveEffect* GetActiveEffectFromActor(Actor* actor);
	bool IsInMenuMode();
}