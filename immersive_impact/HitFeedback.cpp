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
#include "Papyrus.h"
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
#include "BSTHashMap.h"
#include <cstddef>
#include <cstdint>
using std::vector;
using std::pair;

IThreadSafeBasicMemPool<BingleHitWaitNextFrame, 16>	s_bingleHitWaitNextFrameDelegatePool;
IThreadSafeBasicMemPool<BingleStaggerWaitNextFrame, 16>	s_bingleStaggerWaitNextFrameDelegatePool;

template<class Ty>
static inline Ty SafeWrite_Impl(std::size_t addr, Ty data)
{
	DWORD	oldProtect = 0;
	Ty		oldVal = 0;

	if (VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		Ty* p = (Ty*)addr;
		oldVal = *p;
		*p = data;
		VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
	}

	return oldVal;
}

std::uint32_t SafeWrite32uint32(std::uint32_t addr, std::uint32_t data)
{
	return SafeWrite_Impl(addr, data);
}

template <class Ty, class TRet, class... TArg>
inline auto SafeWrite32uint32(UInt32 jumpSrc, TRet(Ty::* fn)(TArg...)) -> decltype(fn)
{
	typedef decltype(fn) Fn;
	union
	{
		UInt32	u32;
		Fn		fn;
	} data;
	data.fn = fn;

	data.u32 = SafeWrite32uint32(jumpSrc, data.u32);
	return data.fn;
}

// HDT patch by himika

class FreezeEventHandler : public BSTEventSink<MenuOpenCloseEvent>
{
public:
	typedef EventResult(FreezeEventHandler::* FnReceiveEvent)(MenuOpenCloseEvent* evn, BSTEventSource<MenuOpenCloseEvent>* src);

	static BSTHashMap<UInt32, FnReceiveEvent> ms_handlers;

	UInt32 GetVPtr() const
	{
		return *(UInt32*)this;
	}

	EventResult ReceiveEvent_Hook(MenuOpenCloseEvent* evn, BSTEventSource<MenuOpenCloseEvent>* src)
	{
		static BSFixedString menuName = "HitFeedbackHelper";

		if (evn->menuName == menuName)
		{
			return kEvent_Continue;
		}

		FnReceiveEvent fn;
		return (ms_handlers.GetAt(GetVPtr(), fn)) ? (this->*fn)(evn, src) : kEvent_Continue;
	}

	void InstallHook()
	{
		UInt32 vptr = GetVPtr();
		FreezeEventHandler::FnReceiveEvent pFn = SafeWrite32uint32(vptr + 4, &FreezeEventHandler::ReceiveEvent_Hook);
		ms_handlers.SetAt(vptr, pFn);
	}
};

BSTHashMap<UInt32, FreezeEventHandler::FnReceiveEvent> FreezeEventHandler::ms_handlers;

class MenuOpenCloseEventSource : public EventDispatcher<MenuOpenCloseEvent>
{
public:
	void ProcessHook()
	{
		lock.Lock();

		BSTEventSink<MenuOpenCloseEvent>* sink;
		UInt32 idx = 0;
		while (eventSinks.GetNthItem(idx, sink))
		{
			const char* className = GetObjectClassName(sink);
			if (strcmp(className, "FreezeEventHandler@@") == 0)
			{
				FreezeEventHandler* freezeEventHandler = static_cast<FreezeEventHandler*>(sink);
				freezeEventHandler->InstallHook();
			}

			++idx;
		}

		lock.Release();
	}

	static void InitHook()
	{
		MenuManager* mm = MenuManager::GetSingleton();
		if (mm)
		{
			MenuOpenCloseEventSource* pThis = static_cast<MenuOpenCloseEventSource*>(mm->MenuOpenCloseEventDispatcher());
			pThis->ProcessHook();
		}
	}
};

HitFeedbackHelper* HitFeedbackHelper::instance = nullptr;

HitFeedbackHelper::HitFeedbackHelper() {
	instance = this;
	InitMovie();
	unk0C = 0x2;
	flags = 0x18902;
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
	MenuOpenCloseEventSource::InitHook();
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

void HitFeedbackHelper::AddStaggerTask(BingleStaggerWaitNextFrame* t) {
	staggerTasks.push_back(std::pair< BingleStaggerWaitNextFrame*, int>(t, 0));
}

void HitFeedbackHelper::Render(){
	if (!EquipWatcher::isInitialized && *g_thePlayer && (Actor*)(*g_thePlayer)->GetNiNode()) {
		EquipWatcher::OnFirstLoad();
	}
	if (staggerTasks.size() > 0) {
		for (vector<pair<BingleStaggerWaitNextFrame*, int>>::iterator
			it = staggerTasks.begin();
			it != staggerTasks.end();) {
			pair<BingleStaggerWaitNextFrame*, int>* p = &(*it);
			if (p->first != nullptr && p->second < 2) {
				p->second++;
				it++;
			}
			else {
				p->first->Run();
				p->first->Dispose();
				it = staggerTasks.erase(it);
			}
		}
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
	CALL_MEMBER_FN(ui, AddMessage)(&StringCache::Ref("HitFeedbackHelper"), UIMessage::kMessage_Open, nullptr);
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

float HitFeedback::deflectChanceMul = 0.0f;
float HitFeedback::deflectChanceMax = 0.0f;
float HitFeedback::staggerResetTime = 0.0f;
float HitFeedback::staggerLimit = 0.0f;
float HitFeedback::staggerDamageMax = 0.0f;
float HitFeedback::staggerAny = 0;
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
		|| (!canKnockdown(target) && target != *g_thePlayer && staggerAny == 0))
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
	float deflectChance = min(armorValue * deflectChanceMul, deflectChanceMax);
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
		BingleHitWaitNextFrame* cmd = BingleHitWaitNextFrame::Create(target, attacker, ae, evn->flags, bowDivider, wep->type());
		HitFeedbackHelper* helper = HitFeedbackHelper::GetInstance();
		if (cmd) {
			helper->SetTask(cmd);
		}
	}
	return kEvent_Continue;
}

BingleHitWaitNextFrame* BingleHitWaitNextFrame::Create(Actor* target, Actor* attacker, ActiveEffect* ae, TESHitEvent::Flags flags, float bowDivider, int wepType){
	BingleHitWaitNextFrame* cmd = s_bingleHitWaitNextFrameDelegatePool.Allocate();
	if (cmd)
	{
		cmd->target = target;
		cmd->attacker = attacker;
		cmd->ae = ae;
		cmd->flags = flags;
		cmd->bowDivider = bowDivider;
		cmd->wepType = wepType;
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
		HitFeedback::lastDamage > -0.5f) 
		return;
	float staggerMagnitude = min(max(-HitFeedback::lastDamage, 25.0f) / HitFeedback::staggerDamageMax * min(flags.powerAttack + flags.bash + 1.0f, 2.0f), 1) / bowDivider / (ae->magnitude + 1);
	if (ae->magnitude >= HitFeedback::staggerLimit) {
		deflectAttack(target, ae, false);
		return;
	}
	else {
		CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerDirection"), 0.5f - (target->rot.z - attacker->rot.z) / M_PI);
		CALL_MEMBER_FN(&(target->animGraphHolder), SetAnimationVariableFloat)(BSFixedString("staggerMagnitude"), staggerMagnitude);
		BingleStaggerWaitNextFrame* cmd = BingleStaggerWaitNextFrame::Create(target);
		HitFeedbackHelper* helper = HitFeedbackHelper::GetInstance();
		if (cmd) {
			target->animGraphHolder.SendAnimationEvent("staggerStop");
			helper->AddStaggerTask(cmd);
		}
	}
	float mulIfDagger = wepType == TESObjectWEAP::GameData::kType_OneHandDagger || wepType == TESObjectWEAP::GameData::kType_1HD ? 0.75f : 1.0f;
	ae->magnitude += 1 * mulIfDagger;
}

void BingleHitWaitNextFrame::Dispose(){
	s_bingleHitWaitNextFrameDelegatePool.Free(this);
}

BingleStaggerWaitNextFrame* BingleStaggerWaitNextFrame::Create(Actor* target) {
	BingleStaggerWaitNextFrame* cmd = s_bingleStaggerWaitNextFrameDelegatePool.Allocate();
	if (cmd)
	{
		cmd->target = target;
	}
	return cmd;
}

void BingleStaggerWaitNextFrame::Run() {
	if (!target ||
		target->flags2.killMove ||
		target->IsDead(1))
		return;
	target->animGraphHolder.SendAnimationEvent("staggerStart");
}

void BingleStaggerWaitNextFrame::Dispose() {
	s_bingleStaggerWaitNextFrameDelegatePool.Free(this);
}