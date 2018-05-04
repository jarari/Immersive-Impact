#include "ActorModifier.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "SKSE/PapyrusEvents.h"

typedef UInt32(*_LookupActorValueByName)(const char * name);
extern const _LookupActorValueByName LookupActorValueByName;

bool restrainEnabled = false;
void ActorModifier::EnableRestraint(bool yes) {
	restrainEnabled = yes;
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
	if (restrain && !isRestrained) {
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
			PlayerControls::GetSingleton()->inputHandlers[1]->enabled = true;
		}
		return;
	}
	if (restrain && !isViewRestrained) {
		isViewRestrained = true;
		PlayerControls::GetSingleton()->inputHandlers[1]->enabled = false;
	} else if (!restrain && isViewRestrained) {
		isViewRestrained = false;
		PlayerControls::GetSingleton()->inputHandlers[1]->enabled = true;
	}
}

void ActorModifier::ModifyAV(Actor * a, char *AVname, float v) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	a->actorValueOwner.SetBase(AVPtr, v - 1.0f);
}

void ActorModifier::SetCurrentAV(Actor * a, char *AVname, float v) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	a->actorValueOwner.SetCurrent(AVPtr, v - 1.0f);
}

float ActorModifier::GetAV(Actor * a, char *AVname) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	return a->actorValueOwner.GetBase(AVPtr);
}

float ActorModifier::GetAVMax(Actor * a, char *AVname) {
	UInt32 AVPtr = LookupActorValueByName(AVname);
	return a->actorValueOwner.GetMaximum(AVPtr);
}
