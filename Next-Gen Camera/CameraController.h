#pragma once
#include <SKSE/GameCamera.h>
#include <SKSE/Utilities.h>
#include <SKSE/GameReferences.h>

class ActorEx : Actor {
public:
	MEMBER_FN_PREFIX(ActorEx);
	DEFINE_MEMBER_FN(GetSaddleOffsetZ, float, 0x006B8800);
}; 

struct bhkCharacterMoveFinishEvent {
	//????
};
DEFINE_EVENTSINK(bhkCharacterMoveFinishEvent);

class CameraPositionUpdater : BSTEventSink<bhkCharacterMoveFinishEvent> {
	virtual ~CameraPositionUpdater();
	virtual	EventResult	ReceiveEvent(bhkCharacterMoveFinishEvent* evn, EventDispatcher<bhkCharacterMoveFinishEvent>* dispatcher) override;
public:
	bool shouldUpdate = false;
	float posX, posY, posZ, lastElapsed;
	TESCameraState* pCamState;
	PlayerCharacter* player;
	ActiveEffect* ae;
};

class CameraController {
protected:
	static CameraController* instance;
	static NiPoint3 camTargetPos;
	static NiPoint3 camVanillaPos;
	static NiPoint3 camCurrentPos;
	static NiPoint3 camVanillaBase;
	static NiPoint3 camBase;
	static float smoothingEnforcer;
	static float offsetX;
	static float offsetZ;
public:
	static CameraPositionUpdater* camUpdater;
	static NiPoint3 lastPlayerPos;
	static NiPoint3 playerVelocity;
	static bool hookActive;
	static void MainBehavior();
	CameraController* GetInstance() {
		return instance;
	}
	static bool ThirdPersonBehavior(ThirdPersonState* pCamState);
	static bool HorseBehavior(HorseCameraState* pCamState);
	static void HookCharacterMoveFinishEvent();
};