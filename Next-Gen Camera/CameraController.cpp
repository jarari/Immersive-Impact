#include "CameraController.h"
#include "ConfigHandler.h"
#include "Utils.h"
#include <SKSE/GameObjects.h>
#include <SKSE/GameRTTI.h>
#include <SKSE/NiNodes.h>
#include <thread>
#include <chrono>

using namespace Utils;

bool CameraController::hookActive = false;
CameraController* CameraController::instance = nullptr;
NiPoint3 CameraController::camTargetPos;
NiPoint3 CameraController::camVanillaPos;
NiPoint3 CameraController::camCurrentPos;
NiPoint3 CameraController::camVanillaBase;
NiPoint3 CameraController::camBase;
NiPoint3 CameraController::lastPlayerPos;
NiPoint3 CameraController::playerVelocity;
float CameraController::smoothingEnforcer;
float CameraController::offsetX;
float CameraController::offsetZ;

//006B8800 (Actor* horse?, return float?)
//fOverShoulderRangedMountedPosX, fOverShoulderRangedMountedAddY, fOverShoulderRangedMountedPosZ when weapon drawn
//TESRace + 0x118, 0x11C, 0x120 when weapon sheathed

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
	smoothingEnforcer = 999.0f;
	ActiveEffect* ae = nullptr;
	float lastElapsed = 0.0f;
	playerVelocity = NiPoint3();
	float lastSprintFov = 0.0f;
	while (running) {
		if (hookActive) {
			PlayerCamera* pCam = PlayerCamera::GetSingleton();
			if (pCam) {
				TESCameraState* pCamState = pCam->cameraState;
				PlayerCharacter* player = *g_thePlayer;
				if (pCamState && player && player->GetNiNode() && pCam->cameraStates) {
					bool processCam = true;
					if (pCamState != lastState) {
						_MESSAGE("State changed");
						lastState = pCamState;
						if(Scale(camCurrentPos) == 0)
							camCurrentPos = player->pos + NiPoint3(0, 0, 121);
						camTargetPos = camCurrentPos;
						lastPlayerPos = player->pos;
						processCam = false;
						ae = GetActiveEffectFromActor(player);
						lastElapsed = ae->elapsed;
						playerVelocity = NiPoint3();
						pCam->worldFOV = pCam->worldFOV + lastSprintFov;
						lastSprintFov = 0.0f;
					}
					bool behaviorExists = false;
					smoothingEnforcer = 999.0f;

					if (ae && ae->elapsed - lastElapsed >= 0.05f) {
						float dElapsed = ae->elapsed - lastElapsed;
						playerVelocity = (player->pos - lastPlayerPos) / dElapsed;
						//TODO: Calculate Velocity
						NiPoint3 l_deltaPlayerPos = WorldToLocal(playerVelocity, NiPoint3(), player->GetNiNode()->m_worldTransform.rot);
						l_deltaPlayerPos.x *= configValues[fCamVelocityStrengthX];
						l_deltaPlayerPos.y *= configValues[fCamVelocityStrengthY];
						l_deltaPlayerPos.z *= configValues[fCamVelocityStrengthZ];
						NiPoint3 w_deltaPlayerPos = LocalToWorld(l_deltaPlayerPos, NiPoint3(), player->GetNiNode()->m_worldTransform.rot);
						playerVelocity = w_deltaPlayerPos;
						lastElapsed = ae->elapsed;
						lastPlayerPos = player->pos;
					}

					if (pCamState == pCam->cameraStates[PlayerCamera::kCameraState_ThirdPerson2]) {
						processCam = ThirdPersonBehavior((ThirdPersonState*)pCamState);
						behaviorExists = true;
					}
					else if (pCamState == pCam->cameraStates[PlayerCamera::kCameraState_Horse]) {
						processCam = HorseBehavior((HorseCameraState*)pCamState);
						behaviorExists = true;
					}
					else {
						camCurrentPos = NiPoint3(*(float*)((UInt32)pCam + 0xB4),
												*(float*)((UInt32)pCam + 0xB8),
												*(float*)((UInt32)pCam + 0xBC));
					}

					if (!behaviorExists || player->flags2.killMove || IsInMenuMode()) {
						processCam = false;
					}


					if (processCam) {
						//Disable sprint effect
						if (((ThirdPersonState*)pCamState)->controllerNode) {
							float SprintFov = ((ThirdPersonState*)pCamState)->controllerNode->m_localTransform.pos.z;
							pCam->worldFOV = pCam->worldFOV + lastSprintFov - SprintFov;
							lastSprintFov = SprintFov;
							//((ThirdPersonState*)pCamState)->controllerNode->m_localTransform.pos.z = 0.0f;
						}
						*(float*)((UInt32)pCamState + 0xA4) = 0.0f;
						*(float*)((UInt32)pCamState + 0xA8) = 0.0f;

						camTargetPos = LocalToWorld(NiPoint3(offsetX * configValues[iShoulder], 0, offsetZ), camTargetPos, pCam->cameraNode->m_worldTransform.rot);
						NiPoint3 deltaCam = (camTargetPos - camCurrentPos) / min(configValues[fCamSmooth], smoothingEnforcer);
						float scale = Scale(deltaCam);
						if (scale < fCamDeltaMin) {
							if (Scale((camTargetPos - camCurrentPos)) > fCamDeltaMin)
								deltaCam = deltaCam / scale * fCamDeltaMin;
							else
								deltaCam = camTargetPos - camCurrentPos;
						}
						camCurrentPos += deltaCam;

						//TODO: Calculate world position -> convert to local position using variables
						NiPoint3 l_currpos = WorldToLocal(camCurrentPos, camVanillaBase, pCam->cameraNode->m_worldTransform.rot);
						NiPoint3 l_vanillapos = WorldToLocal(camVanillaPos, camVanillaBase, pCam->cameraNode->m_worldTransform.rot);
						NiPoint3 l_delta = l_currpos - l_vanillapos;
						*(float*)((UInt32)pCamState + 0x3C) = l_delta.x;
						*(float*)((UInt32)pCamState + 0x40) = l_delta.y;
						*(float*)((UInt32)pCamState + 0x44) = l_delta.z;
					}
				}
			}
		}
		else {
			lastState = nullptr;
			camCurrentPos = NiPoint3();
		}
		std::this_thread::sleep_for(std::chrono::microseconds((int)(1000000 / configValues[iTickRate])));
	}
}

//flags04
enum {
	State_Sneak = 0x200,
	State_AttackStart = 0x10000000,
	State_AttackPre = 0x20000000,
	State_AttackSwing = 0x30000000,
	State_AttackPost = 0x40000000,
	State_Bash = 0x60000000,
	State_BowDrawStart = 0x80000000,
	State_BowDraw = 0x90000000,
	State_BowFullyDrawn = 0xA0000000
};

//flags08
enum {
	State_WeaponIdle = 0x08, //This one has nothing to do with weapon drawn state. Means idle motion playing or not??
	State_WeaponDrawing = 0x40,
	State_WeaponDrawn = 0x60,
	State_WeaponSheating = 0xA0
};

enum {
	State_MagicChannelingStart = 0x2,
	State_MagicChannelingReady = 0x3, //Channeling cast = 0x4, but it shares with casting start?
	State_MagicCastingStart = 0x4,
	State_MagicCasting = 0x6
};

float GetZoom(TESCameraState* st) {
	return *(float*)((UInt32)st + 0x58);
}

bool CompareFlags(UInt32 actorflag, UInt32 flag) {
	return (actorflag & flag) == flag;
}
//CamState + 0x54 = target zoom, 0x58(unk54[1]) = current zoom, 0xA4 = sprint fov [0~1]
//fMinCurrentZoom ~ 1
//Distance from node : 255 at zoom 0, 445/zoom
//CamState + 0x3C = fOverShoulderPosX, 0x48 = current PosX, 0x44 = fOverShoulderPosZ, 0x50 = current PosZ

//ward		concentration 			self		concentration		self		0	0	2	0
//muffle	fire & forget			self		constant efct		self		0	0.5	1	0
//heal		concentration			self		constant efct		self		0	0	2	0
//trans		fire & forget			self		fire & forget		self		0	0.5	1	0
//flame		concentration			aimed		concentration		aimed		0	0	2	2
//light		fire & forget			aimed		fire & forget		aimed		0	0	1	2
//skel		fire & forget			targloc		fire & forget		targloc		0	2	1	4
//																			0x74, 0x78, 0x7C, 0x80
bool CameraController::ThirdPersonBehavior(ThirdPersonState* pCamState) {
	PlayerCharacter* player = *g_thePlayer;
	if (!pCamState->cameraNode || !pCamState->camera)
		return false;
	camVanillaBase = player->pos + NiPoint3(0, 0, pCamState->cameraNode->m_localTransform.pos.z);
	NiPoint3 camFwd;
	GetCameraForward(pCamState->camera, &camFwd);
	float zoomMin = -0.2f;
	Setting* zoomMinSetting = GetINISetting("fMinCurrentZoom");
	if (zoomMinSetting)
		zoomMin = zoomMinSetting->data.f32;
	float zoomCurr = GetZoom(pCamState);
	float zoomLinear = (((zoomCurr - zoomMin) / (1.0f - zoomMin)) * 2.0f + 1.0f); //For later zoom-in effect calculations
	float camDist = 255.0f + 445.0f * zoomCurr;
	float camDistOffset = 0.0f;
	camVanillaPos = camVanillaBase - camFwd * camDist;

	//Sneaking check
	bool standing = (player->actorState.flags04 & State_Sneak) == 0;
	//TODO: Idle
	offsetX = standing ? configValues[fIdleOffsetX] : configValues[fSneakIdleOffsetX];
	offsetZ = standing ? configValues[fIdleOffsetZ] : configValues[fSneakIdleOffsetZ];
	//TODO: Weapon Draw
	if (CompareFlags(player->actorState.flags08, State_WeaponDrawing) ||
		CompareFlags(player->actorState.flags08, State_WeaponDrawn)) {
		offsetX = standing ? configValues[fCombatOffsetX] : configValues[fSneakCombatOffsetX];
		offsetZ = standing ? configValues[fCombatOffsetZ] : configValues[fSneakCombatOffsetZ];
		//TODO: Weapon Swing
		if ((player->actorState.flags08 & State_WeaponIdle) == 0) {
			if (CompareFlags(player->actorState.flags04, State_AttackSwing) ||
				CompareFlags(player->actorState.flags04, State_Bash)) {
				camDistOffset -= 40.0f * zoomLinear;
				smoothingEnforcer = 1.0f;
			}
			else if (CompareFlags(player->actorState.flags04, State_AttackPost)) {
				smoothingEnforcer = 1.0f;
			}
			//TODO: Bow Hold
			//Actor state changes to State_BowDraw when the player pulls out a bow, so we check the idle state.
			else if (CompareFlags(player->actorState.flags04, State_BowDrawStart) ||
					CompareFlags(player->actorState.flags04, State_BowDraw) ||
					CompareFlags(player->actorState.flags04, State_BowFullyDrawn)) {
				offsetX = standing ? configValues[fBowOffsetX] : configValues[fSneakBowOffsetX];
				offsetZ = standing ? configValues[fBowOffsetZ] : configValues[fSneakBowOffsetZ];
				camDistOffset -= 50.0f * zoomLinear;
				smoothingEnforcer = 1.5f;
			}
		}
		//TODO: Magic Cast
		//Casting magic w/ left hand -> place camera on left, and vice versa. Both hands -> center
		float magicOffsetX = 0.0f;
		if (player->magicCaster) {
			bool casting = false; 
			if (player->magicCaster[0] && 
				(CompareFlags(player->magicCaster[0]->unk18, State_MagicCastingStart) || 
					CompareFlags(player->magicCaster[0]->unk18, State_MagicCasting) ||
					CompareFlags(player->magicCaster[0]->unk18, State_MagicChannelingStart) ||
					CompareFlags(player->magicCaster[0]->unk18, State_MagicChannelingReady))) {
				casting = true;
				if (*(UInt32*)((UInt32)player->magicCaster[0] + 0x8C) == 0x8 &&
					(*(TESForm**)((UInt32)player->magicCaster[0]->magicItem + 0x54))->formID != 0x13F45) { //Not dual casting (w/ perk) AND not casting a two-handed spell
					magicOffsetX -= standing ? configValues[fMagicOffsetX] : configValues[fSneakMagicOffsetX];
				}
			}
			if (player->magicCaster[1] && 
				(CompareFlags(player->magicCaster[1]->unk18, State_MagicCastingStart) || 
					CompareFlags(player->magicCaster[1]->unk18, State_MagicCasting) ||
					CompareFlags(player->magicCaster[1]->unk18, State_MagicChannelingStart) ||
					CompareFlags(player->magicCaster[1]->unk18, State_MagicChannelingReady))) {
				casting = true;
				magicOffsetX += standing ? configValues[fMagicOffsetX] : configValues[fSneakMagicOffsetX];
			}
			if (casting) {
				camDistOffset -= 50.0f * zoomLinear;
				smoothingEnforcer = min(1.5f, smoothingEnforcer);
				offsetX = magicOffsetX;
				offsetZ = standing ? configValues[fMagicOffsetZ] : configValues[fSneakMagicOffsetZ];
			}
		}
	}
	camBase = player->pos + NiPoint3(0, 0, pCamState->cameraNode->m_localTransform.pos.z);
	camBase -= playerVelocity / 25.0f;

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
	if (!pCamState->cameraNode || !pCamState->camera)
		return false;
	Actor* horse = GetCurrentHorse();
	if (!horse)
		return false;
	float saddleOffsetZ = CALL_MEMBER_FN((ActorEx*)horse, GetSaddleOffsetZ)();
	camVanillaBase = horse->pos + NiPoint3(0, 0, saddleOffsetZ);
	NiPoint3 camFwd;
	GetCameraForward(pCamState->camera, &camFwd);
	float zoomMin = -0.2f;
	Setting* zoomMinSetting = GetINISetting("fMinCurrentZoom");
	if (zoomMinSetting)
		zoomMin = zoomMinSetting->data.f32;
	float zoomCurr = GetZoom(pCamState);
	float zoomLinear = (((zoomCurr - zoomMin) / (1.0f - zoomMin)) * 2.0f + 1.0f); //For later zoom-in effect calculations
	float camDist = 255.0f + 445.0f * zoomCurr - (*((float*)((UInt32)horse->race + 0x11C)));
	float camDistOffset = 0.0f;
	camVanillaPos = camVanillaBase - camFwd * camDist;

	//TODO: Idle
	offsetX = configValues[fHorseIdleOffsetX];
	offsetZ = configValues[fHorseIdleOffsetZ];
	//TODO: Weapon Draw
	if (CompareFlags(player->actorState.flags08, State_WeaponDrawing) ||
		CompareFlags(player->actorState.flags08, State_WeaponDrawn)) {
		offsetX = configValues[fHorseCombatOffsetX];
		offsetZ = configValues[fHorseCombatOffsetZ];
		//TODO: Weapon Swing
		if (CompareFlags(player->actorState.flags04, State_AttackPre) ||
			CompareFlags(player->actorState.flags04, State_AttackSwing) ||
			CompareFlags(player->actorState.flags04, State_Bash)) {
			camDistOffset -= 40.0f * zoomLinear;
			smoothingEnforcer = 1.0f;
		}
		else if (CompareFlags(player->actorState.flags04, State_AttackPost)) {
			smoothingEnforcer = 1.0f;
		}
		//TODO: Bow Hold
		//Actor state changes to State_BowDraw when the player pulls out a bow, so we check the idle state.
		else if (CompareFlags(player->actorState.flags04, State_BowDrawStart) ||
			CompareFlags(player->actorState.flags04, State_BowDraw) ||
			CompareFlags(player->actorState.flags04, State_BowFullyDrawn)) {
			offsetX = configValues[fHorseBowOffsetX];
			offsetZ = configValues[fHorseBowOffsetZ];
			camDistOffset -= 50.0f * zoomLinear;
			smoothingEnforcer = 1.5f;
		}
	}
	camBase = horse->pos + NiPoint3(0, 0, saddleOffsetZ);
	camBase -= playerVelocity / 25.0f;

	camTargetPos = camBase - camFwd * (camDist + camDistOffset);
	return true;
}