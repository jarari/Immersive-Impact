#include "Utils.h"
#include <SKSE/NiNodes.h>
#include <SKSE/NiObjects.h>
#include <SKSE/GameCamera.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameReferences.h>

using std::vector;
using std::pair;
std::vector<std::pair<Actor*, ActiveEffect*>> storedActiveEffects;

float Utils::Scale(NiPoint3 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void Utils::NormalizeVector(float& x, float& y, float& z) {
	float scale = Scale(NiPoint3(x, y, z));
	if (scale == 0)
		return;
	x /= scale;
	y /= scale;
	z /= scale;
}

void Utils::NormalizeVector(NiPoint3& vec) {
	float scale = Scale(vec);
	if (scale == 0)
		return;
	vec.x /= scale;
	vec.y /= scale;
	vec.z /= scale;
}

void Utils::GetCameraForward(TESCamera* cam, NiPoint3& vecnorm) {
	NiPoint3 fwd(0, 1, 0);
	fwd = cam->cameraNode->m_worldTransform.rot * fwd;
	NormalizeVector(fwd);
	vecnorm.x = fwd.x;
	vecnorm.y = fwd.y;
	vecnorm.z = fwd.z;
}

void Utils::SetMatrix33(float a, float b, float c, float d, float e, float f, float g, float h, float i, NiMatrix33& mat) {
	mat.data[0][0] = a;
	mat.data[0][1] = b;
	mat.data[0][2] = c;
	mat.data[1][0] = d;
	mat.data[1][1] = e;
	mat.data[1][2] = f;
	mat.data[2][0] = g;
	mat.data[2][1] = h;
	mat.data[2][2] = i;
}

NiMatrix33 Utils::GetRotationMatrix33(float pitch, float yaw, float roll) {
	NiMatrix33 m_yaw;
	SetMatrix33(cos(yaw),		-sin(yaw),			0,
				sin(yaw),		cos(yaw),			0,
				0,				0,					1,
				m_yaw);
	NiMatrix33 m_roll;
	SetMatrix33(1,				0,					0,
				0,				cos(roll),			-sin(roll),
				0,				sin(roll),			cos(roll),
				m_roll);
	NiMatrix33 m_pitch;
	SetMatrix33(cos(pitch),		0,					sin(pitch),
				0,				1,					0,
				-sin(pitch),	0,					cos(pitch),
				m_pitch);
	return m_yaw * m_pitch * m_roll;
}

NiMatrix33 Utils::GetRotationMatrix33(NiPoint3 axis, float angle) {
	float x = axis.x * sin(angle / 2.0f);
	float y = axis.y * sin(angle / 2.0f);
	float z = axis.z * sin(angle / 2.0f);
	float w = cos(angle / 2.0f);
	Quaternion q = Quaternion(x, y, z, w);
	return q.ToRotationMatrix33();
}

//Sarrus rule
float Utils::Determinant(NiMatrix33 mat) {
	return mat.data[0][0] * mat.data[1][1] * mat.data[2][2]
		+ mat.data[0][1] * mat.data[1][2] * mat.data[2][0]
		+ mat.data[0][2] * mat.data[1][0] * mat.data[2][1]
		- mat.data[0][2] * mat.data[1][1] * mat.data[2][0]
		- mat.data[0][1] * mat.data[1][0] * mat.data[2][2]
		- mat.data[0][0] * mat.data[1][2] * mat.data[2][1];
}

NiMatrix33 Utils::Inverse(NiMatrix33 mat) {
	float det = Determinant(mat);
	if (det == 0) {
		NiMatrix33 idmat;
		idmat.Identity();
		return idmat;
	}
	float a = mat.data[0][0];
	float b = mat.data[0][1];
	float c = mat.data[0][2];
	float d = mat.data[1][0];
	float e = mat.data[1][1];
	float f = mat.data[1][2];
	float g = mat.data[2][0];
	float h = mat.data[2][1];
	float i = mat.data[2][2];
	NiMatrix33 invmat;
	SetMatrix33(e * i - f * h,			-(b * i - c * h),		b * f - c * e,
				-(d * i - f * g),		a * i - c * g,			-(a * f - c * d),
				d * h - e * g,			-(a * h - b * g),		a * e - b * d, 
				invmat);
	return invmat * (1.0f / det);
}

//(Rotation Matrix)^-1 * (World pos - Local Origin)
NiPoint3 Utils::WorldToLocal(NiPoint3 wpos, NiPoint3 lorigin, NiMatrix33 rot) {
	NiPoint3 lpos = wpos - lorigin;
	NiMatrix33 invrot = Inverse(rot);
	return invrot * lpos;
}

NiPoint3 Utils::LocalToWorld(NiPoint3 lpos, NiPoint3 lorigin, NiMatrix33 rot) {
	return rot * lpos + lorigin;
}

NiPoint3 Utils::Lerp(NiPoint3 start, NiPoint3 end, float t) {
	return start + (end - start) * min(max(t, 0), 1);
}

void Utils::VecToAng(NiPoint3 vec, float& pitch, float& yaw) {
	yaw = atan2(vec.y, vec.x);
	pitch = -asin(vec.z);
}

//By himika
bool Utils::GetNodePosition(TESObjectREFR* ref, const char* nodeName, NiPoint3& point) {
	bool bResult = false;

	if (nodeName[0])
	{
		NiAVObject* object = (NiAVObject*)ref->GetNiNode();
		if (object)
		{
			object = object->GetObjectByName(&nodeName);
			if (object)
			{
				point.x = object->m_worldTransform.pos.x;
				point.y = object->m_worldTransform.pos.y;
				point.z = object->m_worldTransform.pos.z;
				bResult = true;
			}
		}
	}

	return bResult;
}

//By himika, modified for head
bool Utils::GetHeadPos(TESObjectREFR* ref, NiPoint3& point) {
	if (!ref->GetNiNode())
		return false;

	if (ref->formType == kFormType_Character) {
		bool dataFound = false;
		TESRace* race = ((Actor*)ref)->race;
		BGSBodyPartData* bodyPart = race->bodyPartData;
		BGSBodyPartData::Data* data = bodyPart->part[1];
		if (data) {
			if (GetNodePosition(ref, data->unk04.data, point)) {
				dataFound = true;
			}
		}
		if (!dataFound) {
			ref->GetMarkerPosition(&point);
		}
	}
	else {
		point.x = ref->pos.x;
		point.y = ref->pos.y;
		point.z = ref->pos.z;
	}
	return true;
}

ActiveEffect* Utils::GetActiveEffectFromActor(Actor* actor) {
	if (!actor || !actor->GetNiNode())
		return nullptr;
	for (vector<pair<Actor*, ActiveEffect*>>::iterator
		it = storedActiveEffects.begin();
		it != storedActiveEffects.end();) {
		pair<Actor*, ActiveEffect*> p = *it;
		if (p.first && p.second && p.first->IsDead(1) == false && p.second->reference == (TESObjectREFR*)&p.first->magicTarget) {
			if (p.first == actor) {
				_MESSAGE("Got ActiveEffect from stored data.");
				return p.second;
			}
			it++;
		}
		else {
			it = storedActiveEffects.erase(it);
		}
	}
	tList<ActiveEffect>* list_ae = actor->magicTarget.GetActiveEffects();
	tList<ActiveEffect>::Iterator it = list_ae->Begin();
	if (list_ae->Count() == 0)
		return nullptr;
	while (!it.End()) {
		ActiveEffect* ae = it.Get();
		const char* fullname = ae->effect->mgef->fullName.name.data;
		if (fullname != nullptr && strlen(fullname) > 0 && strcmp(fullname, "BingleNGCHelper") == 0) {
			storedActiveEffects.push_back(pair<Actor*, ActiveEffect*>(actor, ae));
			_MESSAGE("Found ActiveEffect from actor.");
			return ae;
		}
		++it;
	}
	return nullptr;
}

bool Utils::IsInMenuMode() {
	return *((int*)0x1B3E428) > 0;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

float Quaternion::Norm() {
	return x * x + y * y + z * z + w * w;
}

//From https://android.googlesource.com/platform/external/jmonkeyengine/+/59b2e6871c65f58fdad78cd7229c292f6a177578/engine/src/core/com/jme3/math/Quaternion.java
NiMatrix33 Quaternion::ToRotationMatrix33() {
	float norm = Norm();
	// we explicitly test norm against one here, saving a division
	// at the cost of a test and branch.  Is it worth it?
	float s = (norm == 1.0f) ? 2.0f : (norm > 0.0f) ? 2.0f / norm : 0;

	// compute xs/ys/zs first to save 6 multiplications, since xs/ys/zs
	// will be used 2-4 times each.
	float xs = x * s;
	float ys = y * s;
	float zs = z * s;
	float xx = x * xs;
	float xy = x * ys;
	float xz = x * zs;
	float xw = w * xs;
	float yy = y * ys;
	float yz = y * zs;
	float yw = w * ys;
	float zz = z * zs;
	float zw = w * zs;

	// using s=2/norm (instead of 1/norm) saves 9 multiplications by 2 here
	NiMatrix33 mat;
	Utils::SetMatrix33(	1 - (yy + zz),		(xy - zw),			(xz + yw),
						(xy + zw),			1 - (xx + zz),		(yz - xw),
						(xz - yw),			(yz + xw),			1 - (xx + yy), 
						mat);

	return mat;
}
