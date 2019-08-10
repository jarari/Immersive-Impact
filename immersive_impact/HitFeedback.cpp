#include "HitFeedback.h"
#include "SKSE/GameReferences.h"
#include "SKSE/GameData.h"
#include <SKSE\PapyrusActor.h>
#include <SKSE\GameRTTI.h>
#include <SKSE\GameExtraData.h>
#include <SKSE\PapyrusEvents.h>
#include <immersive_impact\ActorModifier.h>
#define _USE_MATH_DEFINES
#include <math.h>

HitFeedback* HitFeedback::instance = nullptr;
std::string HitFeedback::className = "HitFeedback";
bool HitFeedback::feedbackEnabled = false;

HitFeedback::~HitFeedback() {
}

void HitFeedback::InitHook() {
	if (instance)
		delete(instance);
	instance = new HitFeedback();
	feedbackEnabled = false;
	g_hitEventSource.AddEventSink((BSTEventSink<EVENT>*)instance);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

void HitFeedback::EnableFeedback(bool b) {
	feedbackEnabled = b;
}

float minForce = 150;
float up = 20;
EventResult HitFeedback::ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) {
	if (!feedbackEnabled)
		return kEvent_Continue;
	if (evn->caster != *g_thePlayer || evn->projectileFormID != 0 || evn->target->formType != kFormType_Character)
		return kEvent_Continue;

	TESObjectWEAP* wep = (TESObjectWEAP*)LookupFormByID(evn->sourceFormID);
	float damageBase = wep->damage.attackDamage;
	Character* target = (Character*)evn->target;
	TESContainer* container = DYNAMIC_CAST(target->baseForm, TESForm, TESContainer);
	ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(target->extraData.GetByType(kExtraData_ContainerChanges));
	tList<ExtraContainerChanges::EntryData>* objList = (tList<ExtraContainerChanges::EntryData>*)pXContainerChanges->data->objList;
	tList<ExtraContainerChanges::EntryData>::Iterator it = objList->Begin();
	float armorValue = 0;
	int armorPieces = 0;
	while (!it.End()) {
		ExtraContainerChanges::EntryData* extraData = it.Get();
		if (extraData && extraData->type->GetFormType() == FormType::kFormType_Armor) {
			TESForm* form = extraData->type;
			int baseCount = 0;
			if (container)
				baseCount = container->CountItem(form);
			if (extraData->countDelta > 0) {
				ExtraContainerChanges::EquipItemData state;
				extraData->GetEquipItemData(state, form->formID, baseCount);
				if (state.isTypeWorn || state.isTypeWornLeft || state.isItemWorn || state.isItemWornLeft) {
					armorValue += ((TESObjectARMO*)form)->armorValTimes100;
					armorPieces++;
				}
			}
		}
		++it;
	}
	armorValue /= 100.0f;
	float knockbackForce = max(min(damageBase * (1.0f - (armorValue * 0.12f + 3 * armorPieces)), 200) * 2, minForce);
	NiPoint3 dir = target->pos - evn->caster->pos;
	dir.z = 0;
	NormalizeVector(dir);
	NiPoint3 pos = target->pos + dir * knockbackForce + NiPoint3(0, 0, up);
	BingleEventInvoker::TranslateToTarget(target);
	BingleEventInvoker::TranslateTo(pos.x, pos.y, pos.z, 3.0f * knockbackForce);
	CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerDirection"), 0.5f - (target->rot.z - evn->caster->rot.z) / M_PI);
	//CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerMagnitude"), knockbackForce / 100.0f);
	CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerMagnitude"), 0.001f);
	target->animGraphHolder.SendAnimationEvent("StaggerStart");
	return kEvent_Continue;
}
