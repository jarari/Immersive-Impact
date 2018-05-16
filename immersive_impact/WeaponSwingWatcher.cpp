#include "WeaponSwingWatcher.h"
#include "SKSE/SafeWrite.h"
#include "SKSE/Utilities.h"
#include "ActorModifier.h"
#include "Papyrus.h"

class WeaponSwingEventSink : public BSTEventSink<WeaponAttack::Event> {
	EventResult ReceiveEvent(WeaponAttack::Event *evn, EventDispatcher<WeaponAttack::Event> *src) {
		return kEvent_Continue;
	}
};

enum {
	Movement_Free = 0,
	Movement_Restrain
};

template<class T, UInt32 Type>
UInt8 CustomHandlerFunctor<Actor, UInt32>::ProcessAction(Actor * actor, UInt32 unk04) {
	ActorModifier::RestrainMovement(actor, Type);
	ActorModifier::RestrainView(actor, Type);
	ActorModifier::ModifyAV(actor, "WeaponSpeedMult", BingleImmersiveImpact::speedValues[BingleImmersiveImpact::ConfigType::Speed_Post] - BingleImmersiveImpact::speedValues[BingleImmersiveImpact::ConfigType::Speed_Offset]);
	ActorModifier::ModifyAV(actor, "LeftWeaponSpeedMult", BingleImmersiveImpact::speedValues[BingleImmersiveImpact::ConfigType::Speed_Post] - BingleImmersiveImpact::speedValues[BingleImmersiveImpact::ConfigType::Speed_LeftOffset]);
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
	WeaponAttack::GetEventSource()->AddEventSink((BSTEventSink<WeaponAttack::Event>*)new WeaponSwingEventSink());
}

void WeaponSwingWatcher::ResetHook() {
}
