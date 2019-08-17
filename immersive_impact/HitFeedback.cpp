#include "HitFeedback.h"
#include "SKSE/GameReferences.h"
#include "SKSE/GameData.h"
#include <SKSE\PapyrusActor.h>
#include <SKSE\GameRTTI.h>
#include <SKSE\GameExtraData.h>
#include <SKSE/Hooks_Threads.h>
#include "BingleEventInvoker.h"
#include "ActorModifier.h"
#include "EquipWatcher.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include "common/IMemPool.h"
#include <SKSE\PluginManager.h>
#include <SKSE/ScaleformLoader.h>
#include <SKSE/ScaleformMovie.h>
#include <SKSE\SafeWrite.h>
using std::vector;
using std::pair;

IThreadSafeBasicMemPool<BingleHitWaitNextFrame, 16>	s_bingleHitWaitNextFrameDelegatePool;

HitFeedbackHelper* HitFeedbackHelper::instance = nullptr;

HitFeedbackHelper::HitFeedbackHelper() {
	instance = this;
	InitMovie();
	unk0C = 0x2;
	flags = 0x2 | 0x800 | 0x1000;
	_MESSAGE("HitFeedbackHelper created");
}

HitFeedbackHelper::~HitFeedbackHelper() {
	_MESSAGE("HitFeedbackHelper destroyed");
}

void HitFeedbackHelper::Register() {
	MenuManager* mm = MenuManager::GetSingleton();
	if (!mm)
		return;
	mm->Register("HitFeedbackHelper", []()->IMenu * { 
		HitFeedbackHelper* helper = new HitFeedbackHelper();
		helper->SetInstance(helper);
		return helper;
		});
	_MESSAGE("HitFeedbackHelper registered");
}

void HitFeedbackHelper::RunTask() {
	if (invoked) {
		invoked = false;
		if (task) {
			if (task->target == t && task->attacker == a) {
				task->Run();
			}
			task->Dispose();
			task = nullptr;
		}
	}
}

void HitFeedbackHelper::SetTask(BingleHitWaitNextFrame* _task) {
	if (task)
		task->Dispose();
	task = _task;
	invoked = true;
}

void HitFeedbackHelper::Render(){
	if (!EquipWatcher::isInitialized && *g_thePlayer && (Actor*)(*g_thePlayer)->GetNiNode()) {
		EquipWatcher::OnFirstLoad();
	}
	else if (EquipWatcher::isInitialized) {
		UIManager* ui = UIManager::GetSingleton();
		CALL_MEMBER_FN(ui, AddMessage)(&StringCache::Ref("HitFeedbackHelper"), UIMessage::kMessage_Close, nullptr);
	}
	IMenu::Render();
}

HitFeedback* HitFeedback::instance = nullptr;
std::string HitFeedback::className = "HitFeedback";
bool HitFeedback::feedbackEnabled = false;
vector<pair<Actor*, ActiveEffect*>> HitFeedback::storedActiveEffects = vector<pair<Actor*, ActiveEffect*>>(32);
float HitFeedback::lastDamage = 0.0f;

HitFeedback::HitFeedback() {
}

HitFeedback::~HitFeedback() {
}

static const UInt32 returnAddr = 0x6AC331;
static void __declspec(naked) Hook_InterceptCalculatedDamage(void) {
	__asm {
		fstp	dword ptr[esp]
		movss	xmm0, [esp]
		movss	[HitFeedback::lastDamage], xmm0
		push	eax
		call	[HitFeedbackHelper::GetInstance]
		cmp		eax, 0
		je		instance_null
		mov		dword ptr[eax + 0x28], esi
		mov		dword ptr[eax + 0x2C], edi
		push	ecx
		mov		ecx, eax
		call	[HitFeedbackHelper::RunTask]
		pop		ecx
		pop		eax
	}
	instance_null:
	__asm {
		push	0x18
		jmp		[returnAddr]
	}
}

void HitFeedback::InitHook() {
	if (instance)
		delete(instance);
	instance = new HitFeedback();
	feedbackEnabled = false;
	g_hitEventSource.AddEventSink((BSTEventSink<EVENT>*)instance);
	WriteRelJump(0x006AC32C, (UInt32)& Hook_InterceptCalculatedDamage);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

void RegisterHelper() {
	HitFeedbackHelper::Register();
	UIManager* ui = UIManager::GetSingleton();
	CALL_MEMBER_FN(ui, AddMessage)(&StringCache::Ref("HitFeedbackHelper"), UIMessage::kMessage_Close, nullptr);
}

void HitFeedback::ResetHook() {
	storedActiveEffects.clear();
	if (!HitFeedbackHelper::GetInstance()) {
		RegisterHelper();
	}
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
	actor->animGraphHolder.SendAnimationEvent("staggerStop");
	if (actor != *g_thePlayer) {
		actor->animGraphHolder.SendAnimationEvent("attackStart");
	}
	if(playSound)
		BingleEventInvoker::PlayDeflectSound(actor);
}

bool canKnockdown(Actor* actor) {
	return ((int)(actor->race->data.raceFlags & TESRace::kRace_NoKnockdowns) - TESRace::kRace_NoKnockdowns);
}

void Dump(void* mem, unsigned int size) {
	char* p = static_cast<char*>(mem);
	unsigned char* up = (unsigned char*)p;
	std::stringstream stream;
	int row = 0;
	for (unsigned int i = 0; i < size; i++){
		stream << std::setfill('0') << std::setw(2) << std::hex << (int)up[i] << " ";
		if (i % 4 == 3) {
			stream << "\t0x" << std::setfill('0') << std::setw(2) << std::hex << (int)up[i] << (int)up[i - 1] << (int)up[i - 2] << (int)up[i - 3];
			stream << "\t0x" << std::hex << row * 4;
			_MESSAGE("%s", stream.str().c_str());
			stream.str(std::string());
			row++;
		}
	}
}

float staggerResetTime = 2.0f;
float deflectChanceMul = 0.1f;
std::random_device rd;
EventResult HitFeedback::ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) {
	if (!feedbackEnabled)
		return kEvent_Continue;
	lastDamage = 0.0f;
	if (!evn->target || evn->target->formType != kFormType_Character || !evn->caster || evn->caster->formType != kFormType_Character)
		return kEvent_Continue;
	Character* target = (Character*)evn->target;
	Character* attacker = (Character*)evn->caster;
	//If the target is dying
	if(attacker->flags2.killMove || target->flags2.killMove || target->IsDead(1))
		return kEvent_Continue;
	ActiveEffect* ae = GetActiveEffectFromActor(target);
	//If the damage is done by magic
	//If the target cannot be knock-downed.
	if (evn->projectileFormID != 0
		|| (!canKnockdown(target) && target != *g_thePlayer))
		return kEvent_Continue;

	if (ae == nullptr)
		return kEvent_Continue;

	if (ae->elapsed - ae->duration > staggerResetTime)
		ae->magnitude = 0;
	ae->duration = ae->elapsed;

	if (evn->flags.blocked && !evn->flags.powerAttack && !evn->flags.bash) {
		deflectAttack(target, ae, false);
		return kEvent_Continue;
	}

	//Dump(evn, 256);
	TESObjectWEAP* wep = (TESObjectWEAP*)LookupFormByID(evn->sourceFormID);
	if (wep == nullptr || wep->formType != kFormType_Weapon)
		return kEvent_Continue;
	float bowDivider = 1.0f;
	if ((wep->type() == TESObjectWEAP::GameData::kType_Bow
		|| wep->type() == TESObjectWEAP::GameData::kType_Bow2
		|| wep->type() == TESObjectWEAP::GameData::kType_CrossBow
		|| wep->type() == TESObjectWEAP::GameData::kType_CBow)
		&& !evn->flags.bash) {
		bowDivider = 4.0f;
		ae->duration = ae->elapsed + 1.0f;
	}
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
	float deflectChance = min(armorValue * deflectChanceMul, 50.0f);
	std::mt19937 e{ rd() }; // or std::default_random_engine e{rd()};
	std::uniform_int_distribution<int> dist{ 0, 99 };
	float chance = dist(e);
	if (chance < deflectChance) {
		deflectAttack(target, ae);
		evn->flags.blocked = true;
		return kEvent_Continue;
	}
	else {
		//float damage = (damageBase + arrowDamage) * (1.0f - (armorValue * 0.12f + 0.03f * armorPieces));
		BingleHitWaitNextFrame* cmd = BingleHitWaitNextFrame::Create(target, attacker, ae, evn->flags, bowDivider);
		HitFeedbackHelper* helper = HitFeedbackHelper::GetInstance();
		if (cmd) {
			helper->SetTask(cmd);
		}
	}
	return kEvent_Continue;
}

BingleHitWaitNextFrame* BingleHitWaitNextFrame::Create(Actor* target, Actor* attacker, ActiveEffect* ae, TESHitEvent::Flags flags, float bowDivider){
	BingleHitWaitNextFrame* cmd = s_bingleHitWaitNextFrameDelegatePool.Allocate();
	if (cmd)
	{
		cmd->target = target;
		cmd->attacker = attacker;
		cmd->ae = ae;
		cmd->flags = flags;
		cmd->bowDivider = bowDivider;
	}
	return cmd;
}

void BingleHitWaitNextFrame::Run(){
	if (!attacker ||
		attacker->flags2.killMove ||
		attacker->IsDead(1) ||
		!target ||
		target->flags2.killMove ||
		target->IsDead(1) ||
		HitFeedback::lastDamage > -0.5) 
		return;
	float staggerMagnitude = min(max(-HitFeedback::lastDamage / 200.0f, 0.125f) * min(flags.powerAttack + flags.bash + 1.0f, 2.0f), 1) / bowDivider / (ae->magnitude + 1);
	if (staggerMagnitude <= 0.005f) {
		deflectAttack(target, ae, false);
		return;
	}
	else {
		CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerDirection"), 0.5f - (target->rot.z - attacker->rot.z) / M_PI);
		CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerMagnitude"), staggerMagnitude);
		target->animGraphHolder.SendAnimationEvent("staggerStart");
	}
	ae->magnitude += 3;
}

void BingleHitWaitNextFrame::Dispose(){
	s_bingleHitWaitNextFrameDelegatePool.Free(this);
}