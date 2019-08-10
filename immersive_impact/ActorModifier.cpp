#include "ActorModifier.h"
#include "EquipWatcher.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/NiNodes.h"
#include "SKSE/NiExtraData.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <SKSE\PapyrusGame.h>
#include <immersive_impact\AimHelperThread.h>
#include <SKSE\GameRTTI.h>

typedef UInt32(*_LookupActorValueByName)(const char * name);
extern const _LookupActorValueByName LookupActorValueByName;

bool isRidingHorse(Actor* a) {
	return (a->actorState.flags04 & (3 << 14));
}

bool restrainEnabled = false;
void ActorModifier::EnableRestraint(bool b) {
	restrainEnabled = b;
	if (!b && (*g_thePlayer)->GetNiNode()) {
		ActorModifier::RestrainMovement(*g_thePlayer, false);
		ActorModifier::RestrainView(*g_thePlayer, false);
	}
}

bool isRestrained = false;
void ActorModifier::RestrainMovement(Actor * a, bool restrain) {
	if (!restrainEnabled) {
		if (isRestrained) {
			isRestrained = false;
			PlayerControls::GetSingleton()->inputHandlers[0]->enabled = true;
		}
		return;
	}
	if (restrain && !isRestrained && !isRidingHorse(*g_thePlayer)) {
		isRestrained = true;
		PlayerControls::GetSingleton()->inputHandlers[0]->enabled = false;
		//BingleEventInvoker::ShowMessageBox(BSFixedString("restrain actor"));
	}
	else if(!restrain && isRestrained){
		isRestrained = false;
		PlayerControls::GetSingleton()->inputHandlers[0]->enabled = true;
		//BingleEventInvoker::ShowMessageBox(BSFixedString("free actor"));
	}
}

bool isViewRestrained = false;
void ActorModifier::RestrainView(Actor * a, bool restrain) {
	if (!restrainEnabled) {
		if (isViewRestrained) {
			isViewRestrained = false;
			PlayerCamera::GetSingleton()->unkD2 = 1;
			//PlayerControls::GetSingleton()->inputHandlers[1]->enabled = true;
		}
		return;
	}
	if (restrain && !isViewRestrained) {
		isViewRestrained = true;
		PlayerCamera::GetSingleton()->unkD2 = 1;
		//PlayerControls::GetSingleton()->inputHandlers[1]->enabled = false;
	} else if (!restrain && isViewRestrained) {
		isViewRestrained = false;
		PlayerCamera::GetSingleton()->unkD2 = 0;
		//PlayerControls::GetSingleton()->inputHandlers[1]->enabled = true;
	}
}

//Ripped from himiko's github
struct UnkCellInfo
{
	UInt32          unk00;
	UInt32          unk04;
	UInt32          unk08;
	UInt32          unk0C;
	UInt32          unk10;
	UInt32          unk14;
	UInt32          unk18;
	UInt32          unk1C;
	UInt32          unk20;
	UInt32	        unk24;
	tArray<UInt32>  actorHandles;	// 28
	tArray<UInt32>  objectHandles;	// 34
};
static UnkCellInfo** playerCellInfo = (UnkCellInfo * *)ADDR_UnkCellInfo;

void NormalizeVector(float& x, float& y, float& z) {
	float scale = sqrt(x * x + y * y + z * z);
	x /= scale;
	y /= scale;
	z /= scale;
}

void NormalizeVector(NiPoint3& vec) {
	float scale = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= scale;
	vec.y /= scale;
	vec.z /= scale;
}

void GetNiNodeForward(NiNode* node, NiPoint3* vecnorm) {
	float x = -node->m_localTransform.rot.data[1][0];
	float y = node->m_localTransform.rot.data[0][0];
	float z = node->m_localTransform.rot.data[2][1];
	NormalizeVector(x, y, z);
	vecnorm->x = x;
	vecnorm->y = y;
	vecnorm->z = z;
}

void GetCameraForward(TESCamera* cam, NiPoint3* vecnorm) {
	GetNiNodeForward(cam->cameraNode, vecnorm);
}

bool GetNodePosition(Actor* actor, const char* nodeName, NiPoint3* point) {
	bool bResult = false;

	if (nodeName[0])
	{
		NiAVObject* object = (NiAVObject*)actor->GetNiNode();
		if (object)
		{
			object = object->GetObjectByName(&nodeName);
			if (object)
			{
				point->x = object->m_worldTransform.pos.x;
				point->y = object->m_worldTransform.pos.y;
				point->z = object->m_worldTransform.pos.z;
				bResult = true;
			}
		}
	}

	return bResult;
}

bool GetTorsoPos(Actor* actor, NiPoint3* point) {
	TESRace* race = actor->race;
	BGSBodyPartData* bodyPart = race->bodyPartData;
	BGSBodyPartData::Data* data;

	// bodyPart->part[0] 胴
	// bodyPart->part[1] 頭
	data = bodyPart->part[0];
	if (data)
	{
		return GetNodePosition(actor, data->unk04.data, point);
	}

	return false;
}

bool GetTargetPos(TESObjectREFR* target, NiPoint3* pos) {
	if (target->GetNiNode() == NULL)
		return false;

	if (target->formType == kFormType_Character)
	{
		if (!GetTorsoPos((Actor*)target, pos))
			target->GetMarkerPosition(pos);
	}
	else
	{
		pos->x = target->pos.x;
		pos->y = target->pos.y;
		pos->z = target->pos.z;
	}

	return true;
}

void SetMatrix33(float a, float b, float c, float d, float e, float f, float g, float h, float i, NiMatrix33& mat) {
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

TargetData FindClosestToAim(float maxAngle, float maxDistance) {
	tArray<UInt32>* actorHandles = &(*playerCellInfo)->actorHandles;
	if (actorHandles == nullptr || actorHandles->count == 0)
		return TargetData();

	UInt32 handle;
	size_t i = 0;
	Actor* a = nullptr;
	float lastLowestScore = 9999;
	UInt16 size = 0;
	float dist = 0;
	NiPoint3 camForward;
	GetCameraForward(PlayerCamera::GetSingleton(), &camForward);
	while (actorHandles->GetNthItem(i++, handle)) {
		TESObjectREFR* ref = NULL;
		if (handle != *g_invalidRefHandle)
			LookupREFRByHandle(&handle, &ref);

		if (ref && ref->formType == kFormType_Character && !ref->IsDead(1))	{
			PlayerCharacter* player = *g_thePlayer;
			//GetTargetPos doesn't work on the player, so we get the target's position and match z value to feet.
			NiPoint3 playerPos = player->pos;
			Actor* target = (Actor*)ref;
			NiPoint3 targetPos = target->pos;
			//GetTargetPos(target, &targetPos);
			//targetPos.z = target->pos.z;
			float dx = targetPos.x - playerPos.x;
			float dy = targetPos.y - playerPos.y;
			float dz = targetPos.z - playerPos.z;
			float dd = sqrt(dx * dx + dy * dy + dz * dz);

			//Arbitrary bounding sphere around the target
			TESBoundObject* targetCasted = DYNAMIC_CAST(target->baseForm, TESForm, TESBoundObject);
			UInt16 bx, by, bx2, by2;
			bx = targetCasted->bounds.x;
			if (bx != 0) {
				by = targetCasted->bounds.y;
				bx2 = targetCasted->bounds2.x;
				by2 = targetCasted->bounds2.y;
				//They're actually int16 values, but SKSE is reading them as UInt16 values so we should convert them.
				bx = bx >= 32768 ? -((int)bx - 65536) : bx;
				by = by >= 32768 ? -((int)by - 65536) : by;
				bx2 = bx2 >= 32768 ? -((int)bx2 - 65536) : bx2;
				by2 = by2 >= 32768 ? -((int)by2 - 65536) : by2;
			}
			else {
				bool bbxExists = false;
				if (target->GetNiNode()->m_extraDataLen > 0) {
					for (int i = 0; i < target->GetNiNode()->m_extraDataLen; i++) {
						NiExtraData* exData = target->GetNiNode()->m_extraData[i];
						if (strcmp(exData->m_pcName, "BBX") == 0) {
							bx = (UInt32)round(*((float*)((char*)exData + 0x18)));
							bx2 = 0;
							by = (UInt32)round(*((float*)((char*)exData + 0x1C)));
							by2 = 0;
							bbxExists = true;
							break;
						}
					}
				}
				if(!bbxExists) {
					//Just put "reasonable" numbers if we couldn't get any bound data.
					bx = 16;
					by = 16;
					bx2 = 16;
					by2 = 16;
				}
			}
			float sizex = (bx + bx2) / 2.0f;
			float sizey = (by + by2) / 2.0f;

			NiPoint3 targetForward;
			float roll = target->rot.z;
			NiMatrix33 m_roll;
			SetMatrix33(cos(roll), -sin(roll), 0,
						sin(roll),	cos(roll),		0,
						0,			0,				1,
						m_roll);
			float pitch = target->rot.x;
			NiMatrix33 m_pitch;
			SetMatrix33(1, 0, 0,
						0,			cos(pitch),		-sin(pitch), 
						0,			sin(pitch),		cos(pitch),
						m_pitch);
			float yaw = target->rot.y;
			NiMatrix33 m_yaw;
			SetMatrix33(cos(yaw), 0, sin(yaw),
						0,			1,				0, 
						-sin(yaw),	0,				cos(yaw),
						m_yaw);
			targetForward = m_yaw * m_pitch * m_roll * NiPoint3(1, 0, 0);
			float pt_dot = targetForward.x * camForward.x + targetForward.y * camForward.y + targetForward.z * camForward.z;
			float pt_ang = std::acos(pt_dot) * 180.0f / M_PI;
			UInt16 r;
			//If the angle between target's forward and cam's forward is bigger than 45 and smaller than 135, 
			//then use x size as bounding sphere size.
			if (isnan(pt_ang) == 0) {
				r = pt_ang > 45 && pt_ang < 135 ? sizey : sizex;
			}
			else {
				r = min(sizex, sizey);
			}
			float actualMaxDistance = maxDistance + r;

			if (dd <= actualMaxDistance && target != player)
			{
				NormalizeVector(dx, dy, dz);
				float dot = dx * camForward.x + dy * camForward.y + dz * camForward.z;
				float ang = std::acos(dot) * 180.0f / M_PI;

				if (isnan(ang) == 0 && ang >= 0 && ang <= maxAngle) {
					float score = dd / actualMaxDistance + ang / maxAngle;
					if (score < lastLowestScore) {
						lastLowestScore = score;
						a = target;
						size = r;
						dist = dd;
					}
				}
			}
		}
	}

	if (a == nullptr)
		return TargetData();

	return TargetData(a, size, dist);
}

float rotateAmount = 1.0f * M_PI / 180.0f;
void RotatePlayerAngleByAmount(float rotZ, float rotX)
{
	PlayerCharacter* player = *g_thePlayer;
	/*TESCameraController* controller = TESCameraController::GetSingleton();
	
	controller->Rotate(player->rot.z, player->rot.x, rotZ, rotX, wait, 0);*/
	//Why rotate cam when view's restrained? Fuck it.
	float drx = rotX - player->rot.x;
	float drz = rotZ - player->rot.z;
	if (drx < -M_PI)
		drx += M_PI * 2.0f;
	else if (drx > M_PI)
		drx -= M_PI * 2.0f;
	drx = max(min(drx, rotateAmount), -rotateAmount);
	if (drz < -M_PI)
		drz += M_PI * 2.0f;
	else if (drz > M_PI)
		drz -= M_PI * 2.0f;
	drz = max(min(drz, rotateAmount), -rotateAmount);
	player->rot.x += drx;
	player->rot.z += drz;
}

// カメラが一人称視点かどうか調べる
bool IsCameraFirstPerson()
{
	PlayerCamera* camera = PlayerCamera::GetSingleton();
	if (!camera)
		return false;

	return camera->cameraState == camera->cameraStates[camera->kCameraState_FirstPerson];
}


// カメラが三人称視点かどうか調べる
bool IsCameraThirdPerson()
{
	PlayerCamera* camera = PlayerCamera::GetSingleton();
	if (!camera)
		return false;

	return camera->cameraState == camera->cameraStates[camera->kCameraState_ThirdPerson2];
}


// カメラの位置を取得
void GetCameraPos(NiPoint3* pos)
{
	PlayerCharacter* player = *g_thePlayer;
	PlayerCamera* camera = PlayerCamera::GetSingleton();
	float x, y, z;

	if (IsCameraFirstPerson() || IsCameraThirdPerson())
	{
		NiNode* node = camera->cameraNode;
		if (node)
		{
			x = node->m_worldTransform.pos.x;
			y = node->m_worldTransform.pos.y;
			z = node->m_worldTransform.pos.z;
		}
	}
	else
	{
		NiPoint3 playerPos;

		player->GetMarkerPosition(&playerPos);
		z = playerPos.z;
		x = player->pos.x;
		y = player->pos.y;
	}

	pos->x = x;
	pos->y = y;
	pos->z = z;
}

// カメラを任意の座標に向ける
static void LookAt(float posX, float posY, float posZ)
{
	PlayerCharacter* player = *g_thePlayer;
	//NiPoint3 cameraPos;
	double x, y, z, xy;
	double rotZ, rotX;

	//GetCameraPos(&cameraPos);

	NiPoint3 playerPos = player->pos;

	x = posX - playerPos.x;
	y = posY - playerPos.y;
	z = posZ - playerPos.z;

	xy = sqrt(x * x + y * y);
	rotZ = atan2(x, y);
	rotX = atan2(-z, xy);

	if (rotZ - player->rot.z > M_PI)
		rotZ -= M_PI * 2;
	else if (rotZ - player->rot.z < -M_PI)
		rotZ += M_PI * 2;


	RotatePlayerAngleByAmount(rotZ, player->rot.x);
}

// プレイヤーを対象に向ける
static void LookAtRef(TESObjectREFR* akRef, float size)
{
	if (akRef == NULL)
		return;
	// akRef->Is3DLoaded()
	if (akRef->GetNiNode() == NULL)
		return;

	//If the target's size is too large, use its position.
	NiPoint3 targetPos;
	if (size < 75) {
		if (!GetTargetPos(akRef, &targetPos))
			return;
	}
	else {
		akRef->GetMarkerPosition(&targetPos);
		targetPos.x = akRef->pos.x;
		targetPos.y = akRef->pos.y;
	}

	LookAt(targetPos.x, targetPos.y, targetPos.z);
}

bool aimHelperEnabled = false;
void ActorModifier::EnableAimHelper(bool b) {
	aimHelperEnabled = b;
}

bool aimLocked = false;
Actor* aimTarget = nullptr;
float maxAng = 70.0f;
void ActorModifier::LockAim(float aimHelperMinDist, float aimHelperMaxDist) {
	PlayerCharacter* player = *g_thePlayer;
	if (!aimHelperEnabled || isRidingHorse(player))
		return;
	TargetData td;
	td = FindClosestToAim(maxAng, aimHelperMaxDist);
	aimTarget = td.target;
	if (aimTarget == nullptr)
		return;
	AimHelperThread::RequestThread(fn<void, Actor*, float>(LookAtRef), aimTarget, td.size);

	//No need to move the player backward
	//The size is multiplied by 2 to prevent the player from teleporting into the target.
	float tpdist = aimHelperMinDist + td.size * 2;
	if (tpdist > td.dd) {
		aimTarget = nullptr;
		return;
	}

	NiPoint3 pos;
	GetTargetPos(player, &pos);
	UInt32 refHandle = player->CreateRefHandle();
	float dx = aimTarget->pos.x - pos.x;
	float dy = aimTarget->pos.y - pos.y;
	float dz = aimTarget->pos.z - pos.z;
	//To prevent player from being stuck in the terrain
	if (dz > 100)
		return;
	NormalizeVector(dx, dy, dz);
	dx *= tpdist;
	dy *= tpdist;
	dz *= tpdist;

	//player->pos = pos;
	//MoveRefrToPosition(player, &refHandle, player->parentCell, CALL_MEMBER_FN(player, GetWorldspace)() , &pos, &(player->rot));
	//x2 at armorWeight 0, x1.1 at armorWeight 100(Heaviest armor set is around 80)
	float vel = 750.0f * (100.0f / (EquipWatcher::playerArmorWeight * 9.5f + 50.0f) + 1.0f);
	BingleEventInvoker::TranslateToTarget(player);
	BingleEventInvoker::TranslateTo(aimTarget->pos.x - dx, aimTarget->pos.y - dy, player->pos.z, vel);

	aimTarget = nullptr;
}

void ActorModifier::UnlockAim() {
	AimHelperThread::StopThread();
}

//A cleaner way to modify actor values.
void ActorModifier::ModifyAV(Actor * a, char *AVname, float v) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	a->actorValueOwner.SetBase(AVPtr, v);
}

void ActorModifier::SetCurrentAV(Actor * a, char *AVname, float v) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	a->actorValueOwner.SetCurrent(AVPtr, v);
}

float ActorModifier::GetAV(Actor * a, char *AVname) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	return a->actorValueOwner.GetCurrent(AVPtr);
}

float ActorModifier::GetAVMax(Actor * a, char *AVname) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	return a->actorValueOwner.GetMaximum(AVPtr);
}