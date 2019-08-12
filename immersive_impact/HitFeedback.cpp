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
#include <time.h>
using std::vector;
using std::pair;

HitFeedback* HitFeedback::instance = nullptr;
std::string HitFeedback::className = "HitFeedback";
bool HitFeedback::feedbackEnabled = false;
vector<pair<Actor*, ActiveEffect*>> HitFeedback::storedActiveEffects = vector<pair<Actor*, ActiveEffect*>>(32);

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

void HitFeedback::ResetHook() {
	storedActiveEffects.clear();
}

void HitFeedback::EnableFeedback(bool b) {
	feedbackEnabled = b;
}

ActiveEffect* GetActiveEffectFromActor(Actor* actor) {
	for (vector<pair<Actor*, ActiveEffect*>>::iterator	
		it = HitFeedback::storedActiveEffects.begin();
		it != HitFeedback::storedActiveEffects.end();) {
		pair<Actor*, ActiveEffect*> p = *it;
		if (p.first != nullptr && p.first->IsDead(1) == false) {
			if (p.first == actor) {
				return p.second;
			}
			it++;
		}
		else {
			it = HitFeedback::storedActiveEffects.erase(it);
		}
	}
	tList<ActiveEffect>* list_ae = actor->magicTarget.GetActiveEffects();
	tList<ActiveEffect>::Iterator it = list_ae->Begin();
	if (list_ae->Count() == 0)
		return nullptr;
	while (!it.End()) {
		ActiveEffect* ae = it.Get();
		const char* fullname = ae->effect->mgef->fullName.name.data;
		if (fullname != nullptr && strlen(fullname) > 0 && strcmp(fullname, "BingleHitFeedback") == 0) {
			HitFeedback::storedActiveEffects.push_back(pair<Actor*, ActiveEffect*>(actor, ae));
			return ae;
		}
		++it;
	}
	return nullptr;
}

void deflectAttack(Actor* actor, ActiveEffect* ae, bool playSound = true) {
	//ae->magnitude = 0;
	//ae->duration = ae->elapsed;
	if (actor != *g_thePlayer) {
		actor->animGraphHolder.SendAnimationEvent("bleedOutStop");
		actor->animGraphHolder.SendAnimationEvent("attackStart");
	}
	BingleEventInvoker::PlayDeflectSound(actor);
}

float staggerResetTime = 2.0f;
float deflectChanceMul = 0.1f;
EventResult HitFeedback::ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) {
	if (!feedbackEnabled)
		return kEvent_Continue;
	if (evn->projectileFormID != 0 || evn->target->formType != kFormType_Character)
		return kEvent_Continue;
	Character* target = (Character*)evn->target;
	if((evn->caster->formType == kFormType_Character && ((Character*)(evn->caster))->flags2.killMove) || target->flags2.killMove || target->IsDead(1))
		return kEvent_Continue;
	if (!(target->race->data.raceFlags & TESRace::kRace_NoKnockdowns - TESRace::kRace_NoKnockdowns) && target != *g_thePlayer)
		return kEvent_Continue;

	ActiveEffect* ae = GetActiveEffectFromActor(target);
	if (ae == nullptr)
		return kEvent_Continue;

	if (evn->flags.blocked) {
		deflectAttack(target, ae);
		return kEvent_Continue;
	}

	if (ae->elapsed - ae->duration > staggerResetTime)
		ae->magnitude = 0;

	TESObjectWEAP* wep = (TESObjectWEAP*)LookupFormByID(evn->sourceFormID);
	if (wep == nullptr)
		return kEvent_Continue;
	float bowDivider = 1.0f;
	if (wep->type() == TESObjectWEAP::GameData::kType_Bow
		|| wep->type() == TESObjectWEAP::GameData::kType_Bow2
		|| wep->type() == TESObjectWEAP::GameData::kType_CrossBow
		|| wep->type() == TESObjectWEAP::GameData::kType_CBow)
		bowDivider = 2.0f;
	float damageBase = wep->damage.attackDamage;
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
	float deflectChance = armorValue * deflectChanceMul;
	srand(time(NULL));
	float chance = rand() % 100;
	if (chance < deflectChance) {
		deflectAttack(target, ae);
		return kEvent_Continue;
	}
	else {
		float damage = damageBase * (1.0f - (armorValue * 0.12f + 3 * armorPieces));
		float staggerMagnitude = min(max(damage / 200.0f, 0.05f) * min(evn->flags.powerAttack + evn->flags.bash + 1.0f, 2.0f), 1) / bowDivider / (ae->magnitude + 1);
		if (staggerMagnitude <= 0.003125f) {
			deflectAttack(target, ae, false);
			return kEvent_Continue;
		}
		else {
			target->animGraphHolder.SendAnimationEvent("bleedOutStop");
			CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerDirection"), 0.5f - (target->rot.z - evn->caster->rot.z) / M_PI);
			CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerMagnitude"), staggerMagnitude);
			target->animGraphHolder.SendAnimationEvent("staggerStart");
		}
		ae->magnitude += 3;
		ae->duration = ae->elapsed;
	}
	return kEvent_Continue;
}
