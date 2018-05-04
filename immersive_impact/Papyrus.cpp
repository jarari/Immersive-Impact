#include "Papyrus.h"
#include "SKSE/PapyrusNativeFunctions.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "SKSE/PapyrusEvents.h"
#include "ActorModifier.h"

typedef TESForm * (*_LookupFormByID)(UInt32 id);
extern const _LookupFormByID LookupFormByID;

namespace BingleImmersiveImpact {
	static TESForm *unarmed;
	void EvaluateTypes(StaticFunctionTag *base, BSFixedString event, float pre, float swing1h, float swing2h, float swingdag, float swingfist, float post) {
		BSFixedString s_pre("preHitFrame");
		BSFixedString s_sw("weaponSwing");
		BSFixedString s_lsw("weaponLeftSwing");
		BSFixedString s_post("AttackWinStart");
		//_MESSAGE("Func ran");
		if (event == s_pre) {
			//Tried OnActorAction, but it sucks. I figured this way is more reliable.
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), 1);
			ActorModifier::RestrainView((Actor*)(*g_thePlayer), 1);

			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", pre);
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", pre);
			//_MESSAGE("pre %f", pre);
		} else if (event == s_sw || event == s_lsw) {
			int weptype = 0;
			if (event == s_sw && papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1))
				weptype = ((TESObjectWEAP*)(papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1)))->type();
			else if(event == s_lsw && papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0))
				weptype = ((TESObjectWEAP*)(papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0)))->type();
			if (weptype == 5 || weptype == 6) {
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", swing2h);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", swing2h);
				//_MESSAGE("2h %f", swing2h);
			} else if (weptype == 1 || weptype == 3 || weptype == 4) {
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", swing1h);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", swing1h);
				//_MESSAGE("1h %f", swing1h);
			} else if (weptype == 2) {
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", swingdag);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", swingdag);
				//_MESSAGE("dag %f", swingdag);
			} else if (weptype == 0) {
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", swingfist);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", swingfist);
				//_MESSAGE("fist %f", swingfist);
			}
		} else if (event == s_post) {
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), 0);

			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", post);
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", post);
			//_MESSAGE("post %f", post);
		}
	}
	void RegisterForFistRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for fist request event.");
		g_fistRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void RegisterForInitRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for init request event.");
		g_initRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void RegisterForNotificationRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for notifcation request event.");
		g_notiRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void RegisterForMessageBoxRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for messagebox request event.");
		g_msgboxRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void RestrainMovement(StaticFunctionTag *base, Actor* a, UInt32 i) {
		ActorModifier::RestrainMovement(a, i);
	}

	void EnableRestrainMovement(StaticFunctionTag *base) {
		ActorModifier::EnableRestraint(true);
	}

	void DisableRestrainMovement(StaticFunctionTag *base) {
		ActorModifier::EnableRestraint(false);
	}
}


bool Papyrus::RegisterFuncs(VMClassRegistry * registry) {
	registry->RegisterFunction(
		new NativeFunction7 <StaticFunctionTag, void, BSFixedString, float, float, float, float, float, float>("EvaluateTypes", "BingleImmersiveFeedback", BingleImmersiveImpact::EvaluateTypes, registry));
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForFistRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForFistRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForFistRequest", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForInitRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForInitRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForInitRequest", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForNotificationRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForNotificationRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForNotificationRequest", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForMessageBoxRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForMessageBoxRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForMessageBoxRequest", VMClassRegistry::kFunctionFlag_NoWait);

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, Actor*, UInt32>("RestrainMovement", "BingleImmersiveFeedback", BingleImmersiveImpact::RestrainMovement, registry));
	
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("EnableRestrainMovement", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::EnableRestrainMovement, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("DisableRestrainMovement", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::DisableRestrainMovement, registry));
	return true;
}
