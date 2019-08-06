#include "ActorModifier.h"
#include "EquipWatcher.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/NiNodes.h"
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
		CALL_MEMBER_FN(&(a->animGraphHolder), SetAnimationVariableFloat)("Speed", 0);
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


void NormalizeVector(float &x, float &y, float &z) {
	float scale = sqrt(x * x + y * y + z * z);
	x /= scale;
	y /= scale;
	z /= scale;
}

void NormalizeVector(NiPoint3 &vec) {
	float scale = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= scale;
	vec.y /= scale;
	vec.z /= scale;
}

void GetCameraForward(TESCamera* cam, NiPoint3* vecnorm) {
	NiMatrix33 transform = cam->cameraNode->m_localTransform.rot;
	float x = -transform.data[1][0];
	float y = transform.data[0][0];
	float z = transform.data[2][1];
	NormalizeVector(x, y, z);
	vecnorm->x = x;
	vecnorm->y = y;
	vecnorm->z = z;
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

TargetData FindClosestToAim(float maxAngle, float minDistance, float maxDistance) {
	tArray<UInt32>* actorHandles = &(*playerCellInfo)->actorHandles;
	if (actorHandles->count == 0)
		return TargetData();

	UInt32 handle;
	size_t i = 0;
	Actor* a = nullptr;
	float lastLowestScore = 9999;
	float size = 0;
	while (actorHandles->GetNthItem(i++, handle)) {
		TESObjectREFR* ref = NULL;
		if (handle != *g_invalidRefHandle)
			LookupREFRByHandle(&handle, &ref);

		if (ref && ref->formType == kFormType_Character && !ref->IsDead(1))	{
			PlayerCharacter* player = *g_thePlayer;
			NiPoint3 playerPos;
			GetTargetPos(player, &playerPos);
			Actor* target = (Actor*)ref;
			NiPoint3 targetPos;
			GetTargetPos(target, &targetPos);
			float dx = targetPos.x - playerPos.x;
			float dy = targetPos.y - playerPos.y;
			float dz = targetPos.z - playerPos.z;
			float dd = sqrt(dx * dx + dy * dy + dz * dz);

			//Arbitrary bounding sphere around the target
			TESNPC* targetCasted = DYNAMIC_CAST(target->baseForm, TESForm, TESNPC);
			UInt16 bx = targetCasted->bounds.x;
			UInt16 by = targetCasted->bounds.y;
			//They're actually int16 values, but SKSE is reading them as UInt16 values so we should convert them.
			int r = min(bx >= 32768 ? 65535 - bx : bx,
				by >= 32768 ? 65535 - by : by);
			float actualMaxDistance = maxDistance + r;

			if (dd >= minDistance && dd <= actualMaxDistance && target != player)
			{
				NiPoint3 camForward;
				GetCameraForward(PlayerCamera::GetSingleton(), &camForward);
				NormalizeVector(dx, dy, dz);
				float dot = dx * camForward.x + dy * camForward.y + dz * camForward.z;
				float ang = std::acos(dot) * 180.0f / M_PI;

				if (isnan(ang) == 0 && ang >= 0 && ang <= maxAngle) {
					float score = dd / actualMaxDistance + ang / maxAngle;
					if (score < lastLowestScore) {
						lastLowestScore = score;
						a = target;
						size = r;
					}
				}
			}
		}
	}

	if (a == nullptr)
		return TargetData();

	return TargetData(a, size);
}

static void SetPlayerAngle(float rotZ, float rotX, float wait)
{
	PlayerCharacter* player = *g_thePlayer;
	TESCameraController* controller = TESCameraController::GetSingleton();

	if (wait < 20)
		wait = 20;

	controller->Rotate(player->rot.z, player->rot.x, rotZ, rotX, wait, 0);
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
static void LookAt(float posX, float posY, float posZ, float wait)
{
	PlayerCharacter* player = *g_thePlayer;
	NiPoint3 cameraPos;
	double x, y, z, xy;
	double rotZ, rotX;

	GetCameraPos(&cameraPos);

	x = posX - cameraPos.x;
	y = posY - cameraPos.y;
	z = posZ - cameraPos.z;

	xy = sqrt(x * x + y * y);
	rotZ = atan2(x, y);
	rotX = atan2(-z, xy);

	if (rotZ - player->rot.z > M_PI)
		rotZ -= M_PI * 2;
	else if (rotZ - player->rot.z < -M_PI)
		rotZ += M_PI * 2;

	SetPlayerAngle(rotZ, player->rot.x, wait);
}

// プレイヤーを対象に向ける
static void LookAtRef(TESObjectREFR* akRef, float wait)
{
	if (akRef == NULL)
		return;
	// akRef->Is3DLoaded()
	if (akRef->GetNiNode() == NULL)
		return;

	NiPoint3 targetPos;
	if (!GetTargetPos(akRef, &targetPos))
		return;

	LookAt(targetPos.x, targetPos.y, targetPos.z, wait);
}

bool aimHelperEnabled = false;
void ActorModifier::EnableAimHelper(bool b) {
	aimHelperEnabled = b;
}


bool aimLocked = false;
Actor* aimTarget = nullptr;
float maxAng = 60.0f;
void ActorModifier::LockAim(float aimHelperMinDist, float aimHelperMaxDist) {
	PlayerCharacter* player = *g_thePlayer;
	if (!aimHelperEnabled || isRidingHorse(player))
		return;
	TargetData td;
	td = FindClosestToAim(maxAng, aimHelperMinDist, aimHelperMaxDist);
	aimTarget = td.target;
	if (aimTarget == nullptr)
		return;
	AimHelperThread::RequestThread(fn<void, Actor*, float>(LookAtRef), aimTarget, 50);

	NiPoint3 pos;
	GetTargetPos(player, &pos);
	UInt32 refHandle = player->CreateRefHandle();
	float dx = aimTarget->pos.x - pos.x;
	float dy = aimTarget->pos.y - pos.y;
	float dz = aimTarget->pos.z - pos.z;
	NormalizeVector(dx, dy, dz);
	dx *= td.size;
	dy *= td.size;
	dz *= td.size;

	//player->pos = pos;
	//MoveRefrToPosition(player, &refHandle, player->parentCell, CALL_MEMBER_FN(player, GetWorldspace)() , &pos, &(player->rot));
	BingleEventInvoker::TranslateTo(aimTarget->pos.x - dx, aimTarget->pos.y - dy, player->pos.z, EquipWatcher::playerArmorWeight);

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
