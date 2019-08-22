#pragma once
#include <SKSE/GameCamera.h>
#include <SKSE/Utilities.h>
#include <SKSE/GameReferences.h>

class ActorEx : Actor {
public:
	MEMBER_FN_PREFIX(ActorEx);
	DEFINE_MEMBER_FN(GetSaddleOffsetZ, float, 0x006B8800);
};

class CameraController {
protected:
	static CameraController* instance;
	static NiPoint3 camTargetPos;
	static NiPoint3 camVanillaPos;
	static NiPoint3 camCurrentPos;
	static NiPoint3 camVanillaBase;
	static NiPoint3 camBase;
	static NiPoint3 lastPlayerPos;
public:
	static int tickRate;
	static void MainBehavior();
	CameraController* GetInstance() {
		return instance;
	}
	static bool ThirdPersonBehavior(ThirdPersonState* pCamState);
	static bool HorseBehavior(HorseCameraState* pCamState);
};