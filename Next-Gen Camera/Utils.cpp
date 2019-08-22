#include "Utils.h"
#include <SKSE/NiNodes.h>
#include <SKSE/NiObjects.h>
#include <SKSE/GameCamera.h>

float Utils::Scale(NiPoint3 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void Utils::NormalizeVector(float& x, float& y, float& z) {
	float scale = Scale(NiPoint3(x, y, z));
	x /= scale;
	y /= scale;
	z /= scale;
}

void Utils::NormalizeVector(NiPoint3& vec) {
	float scale = Scale(vec);
	vec.x /= scale;
	vec.y /= scale;
	vec.z /= scale;
}

void Utils::GetCameraForward(TESCamera* cam, NiPoint3* vecnorm) {
	NiPoint3 fwd(0, 1, 0);
	fwd = cam->cameraNode->m_worldTransform.rot * fwd;
	NormalizeVector(fwd);
	vecnorm->x = fwd.x;
	vecnorm->y = fwd.y;
	vecnorm->z = fwd.z;
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
