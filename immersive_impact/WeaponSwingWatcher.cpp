#include "WeaponSwingWatcher.h"
#include "SKSE/SafeWrite.h"
#include "SKSE/Utilities.h"
#include "ActorModifier.h"
#include "Papyrus.h"
#include "BingleEventInvoker.h"
#include "ConfigHandler.h"

//This class is used to free the player from restraints if his attack got canceled.

enum {
	Movement_Free = 0,
	Movement_Restrain
};

using namespace BingleImmersiveImpact;
template<class T, UInt32 Type>
UInt8 CustomHandlerFunctor<Actor, UInt32>::ProcessAction(Actor * actor, UInt32 unk04) {
	if (actor != (Actor*)*g_thePlayer)
		return CALL_MEMBER_FN(static_cast<T*>(this), Process_Origin)(actor, unk04);
	_MESSAGE("AttackStop");
	ActorModifier::RestrainMovement(actor, false);
	//ActorModifier::RestrainView(actor, false);
	ActorModifier::UnlockAim();
	BingleEventInvoker::TranslateToTarget(actor);
	BingleEventInvoker::StopTranslation();
	if (isSpeedAdjustmentEnabled()) {
		ActorModifier::SetCurrentAV(actor, "WeaponSpeedMult", configValues[ConfigType::Speed_Pre] + configValues[ConfigType::Speed_Offset]);
		ActorModifier::SetCurrentAV(actor, "LeftWeaponSpeedMult", configValues[ConfigType::Speed_Pre] + configValues[ConfigType::Speed_LeftOffset]);
	}
	_MESSAGE("AttackStop end");
	return CALL_MEMBER_FN(static_cast<T*>(this), Process_Origin)(actor, unk04);
}

WeaponSwingWatcher *WeaponSwingWatcher::instance = nullptr;

WeaponSwingWatcher::~WeaponSwingWatcher() {
}

void WeaponSwingWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new WeaponSwingWatcher();
	SafeWrite32(0x010D5444 + 0x04, GetFnAddr(&CustomHandlerFunctor<Actor, UInt32>::ProcessAction<AttackStopHandler, Movement_Free>));
}

void WeaponSwingWatcher::ResetHook() {
}