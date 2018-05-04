#include "WeaponSwingWatcher.h"
#include "SKSE/SafeWrite.h"
#include "SKSE/Utilities.h"
#include "ActorModifier.h"

enum {
	Movement_Free = 0,
	Movement_Restrain
};

template<class T, UInt32 Type>
UInt8 CustomHandlerFunctor<Actor, UInt32>::ProcessAction(Actor * actor, UInt32 unk04) {
	ActorModifier::RestrainMovement(actor, Type);
	ActorModifier::RestrainView(actor, Type);
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
	SafeWrite32(0x010D542C + 0x04, GetFnAddr(&CustomHandlerFunctor<Actor, UInt32>::ProcessAction<AttackWinStartHandler, Movement_Free>));
}

void WeaponSwingWatcher::ResetHook() {
}
