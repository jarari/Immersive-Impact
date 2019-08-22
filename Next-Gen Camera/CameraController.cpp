#include "CameraController.h"
#include "Utils.h"
#include <SKSE/NiNodes.h>
#include <thread>
#include <chrono>

using namespace Utils;

int CameraController::tickRate = 66;
CameraController* CameraController::instance = nullptr;
NiPoint3 CameraController::camTargetPos;
NiPoint3 CameraController::camVanillaPos;
NiPoint3 CameraController::camCurrentPos;
NiPoint3 CameraController::camVanillaBase;
NiPoint3 CameraController::camBase;
NiPoint3 CameraController::lastPlayerPos;

//006B8800 (Actor* horse?, return float?)
//fOverShoulderRangedMountedPosX, fOverShoulderRangedMountedAddY, fOverShoulderRangedMountedPosZ when weapon drawn
//TESRace + 0x118, 0x11C, 0x120 when weapon sheathed

float fCamSmooth = 7.5f;
float fCamDeltaMin = 3.0f;

void CameraController::MainBehavior() {
	_MESSAGE("Thread start");
	if (instance)
		delete(instance);
	instance = new CameraController();
	bool running = true;
	TESCameraState* lastState = nullptr;
	camTargetPos = NiPoint3();
	camVanillaPos = NiPoint3();
	camCurrentPos = NiPoint3();
	camVanillaBase = NiPoint3();
	camBase = NiPoint3();
	lastPlayerPos = NiPoint3();
	while (running) {
		PlayerCamera* pCam = PlayerCamera::GetSingleton();
		if (pCam) {
			TESCameraState* pCamState = pCam->cameraState; 
			PlayerCharacter* player = *g_thePlayer;
			if (pCamState && player && player->GetNiNode()) {
				if (pCamState != lastState) {
					_MESSAGE("State changed");
					std::this_thread::sleep_for(std::chrono::microseconds(1000000));
					lastState = pCamState;
					if(Scale(camCurrentPos) == 0)
						camCurrentPos = player->pos + NiPoint3(0, 0, 121);
					camTargetPos = camCurrentPos;
					lastPlayerPos = player->pos;
					continue;
				}
				bool processCam = true;
				bool behaviorExists = false;
				if (pCamState == pCam->cameraStates[PlayerCamera::kCameraState_ThirdPerson2]) {
					processCam = ThirdPersonBehavior((ThirdPersonState*)pCamState);
					behaviorExists = true;
				}
				else if (pCamState == pCam->cameraStates[PlayerCamera::kCameraState_Horse]) {
					processCam = HorseBehavior((HorseCameraState*)pCamState);
					behaviorExists = true;
				}

				if (!behaviorExists || player->flags2.killMove) {
					processCam = false;
					_MESSAGE("Don't process");
				}


				if (processCam) {
					NiPoint3 deltaCam = (camTargetPos - camCurrentPos) / fCamSmooth;
					float scale = Scale(deltaCam);
					if (scale < fCamDeltaMin) {
						if (Scale((camTargetPos - camCurrentPos)) > fCamDeltaMin)
							deltaCam = deltaCam / scale * fCamDeltaMin;
						else
							deltaCam = camTargetPos - camCurrentPos;
					}
					camCurrentPos += deltaCam;

					//TODO: Calculate world position -> convert to local position using variables
					NiMatrix33 playerRot = GetRotationMatrix33(player->rot.x, player->rot.y, player->rot.z);
					NiPoint3 l_currpos = WorldToLocal(camCurrentPos, camVanillaBase, playerRot);
					NiPoint3 l_vanillapos = WorldToLocal(camVanillaPos, camVanillaBase, playerRot);
					NiPoint3 l_delta = l_currpos - l_vanillapos;
					*(float*)((UInt32)pCamState + 0x3C) = l_delta.x;
					*(float*)((UInt32)pCamState + 0x40) = l_delta.y;
					*(float*)((UInt32)pCamState + 0x44) = l_delta.z;
				}
				else {
					*(float*)((UInt32)pCamState + 0x3C) = 0;
					*(float*)((UInt32)pCamState + 0x40) = 0;
					*(float*)((UInt32)pCamState + 0x44) = 0;
				}
				lastPlayerPos = player->pos;
			}
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1000000 / tickRate));
	}
}

float fIdleOffsetX = 25.0f;
float fIdleOffsetZ = 15.0f;
float fCombatOffsetX = 25.0f;
float fCombatOffsetZ = 15.0f;
float fMagicOffsetX = 25.0f;
float fMagicOffsetZ = 15.0f;
float fBowOffsetX = 25.0f;
float fBowOffsetZ = 15.0f;

float fHorseIdleOffsetX = 35.0f;
float fHorseIdleOffsetZ = 15.0f;
float fHorseCombatOffsetX = 35.0f;
float fHorseCombatOffsetZ = 15.0f;
float fHorseMagicOffsetX = 35.0f;
float fHorseMagicOffsetZ = 15.0f;
float fHorseBowOffsetX = 35.0f;
float fHorseBowOffsetZ = 15.0f;

float fCamVelocityStrength = 3.0f;

float GetZoom(TESCameraState* st) {
	return *(float*)((UInt32)st + 0x58);
}

//CamState + 0x54 = target zoom, 0x58(unk54[1]) = current zoom, 0xA4 = sprint fov [0~1]
//fMinCurrentZoom ~ 1
//Distance from node : 255 at zoom 0, 445/zoom
//CamState + 0x3C = fOverShoulderPosX, 0x48 = current PosX, 0x44 = fOverShoulderPosZ, 0x50 = current PosZ
bool CameraController::ThirdPersonBehavior(ThirdPersonState* pCamState) {
	PlayerCharacter* player = *g_thePlayer;
	if (!pCamState->cameraNode)
		return false;
	camVanillaBase = player->pos + NiPoint3(0, 0, pCamState->cameraNode->m_localTransform.pos.z);
	camBase = LocalToWorld(NiPoint3(fIdleOffsetX, 0, fIdleOffsetZ), player->pos, GetRotationMatrix33(player->rot.x, player->rot.y, player->rot.z))
		+ NiPoint3(0, 0, pCamState->cameraNode->m_localTransform.pos.z);
	NiPoint3 camFwd;
	GetCameraForward(pCamState->camera, &camFwd);
	float camDist = 255.0f + 445.0f * GetZoom(pCamState);
	camVanillaPos = camVanillaBase - camFwd * camDist;
	float camDistOffset = 0.0f;
	//TODO: Add Velocity
	NiPoint3 deltaPlayerPos = player->pos - lastPlayerPos;
	camBase -= deltaPlayerPos * fCamVelocityStrength;
	//TODO: Idle
	//TODO: Weapon Draw
	//TODO: Weapon Swing
	//TODO: Bow Hold
	//TODO: Magic Cast
	camTargetPos = camBase - camFwd * (camDist + camDistOffset);
	return true;
}

//Only works when current state == HorseCameraState
Actor* GetCurrentHorse() {
	UInt32 unk = *((UInt32*)((UInt32)PlayerCamera::GetSingleton() + 0x2C));
	return (Actor*)(*(UInt32*)((unk & 0xFFFFF) * 0x8 + 0x131063C) - 0x14);
}

bool CameraController::HorseBehavior(HorseCameraState* pCamState) {
	PlayerCharacter* player = *g_thePlayer;
	if (!pCamState->cameraNode)
		return false;
	Actor* horse = GetCurrentHorse();
	if (!horse)
		return false;
	float saddleOffsetZ = CALL_MEMBER_FN((ActorEx*)horse, GetSaddleOffsetZ)();
	camVanillaBase = player->pos + NiPoint3(0, 0, saddleOffsetZ);
	camBase = LocalToWorld(NiPoint3(fHorseIdleOffsetX, 0, fHorseIdleOffsetZ), horse->pos, GetRotationMatrix33(horse->rot.x, horse->rot.y, horse->rot.z))
		+ NiPoint3(0, 0, saddleOffsetZ);
	NiPoint3 camFwd;
	GetCameraForward(pCamState->camera, &camFwd);
	float camDist = 255.0f + 445.0f * GetZoom(pCamState) - (*((float*)((UInt32)horse->race + 0x11C)));
	camVanillaPos = camVanillaBase - camFwd * camDist;
	//TODO: Add Velocity
	NiPoint3 deltaPlayerPos = player->pos - lastPlayerPos;
	camBase -= deltaPlayerPos * fCamVelocityStrength;
	camTargetPos = camBase - camFwd * camDist;
	return true;
}