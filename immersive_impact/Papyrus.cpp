#include "Papyrus.h"
#include "SKSE/PapyrusNativeFunctions.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "SKSE/PapyrusEvents.h"
#include "ActorModifier.h"
#include "ConfigHandler.h"
#include "EquipWatcher.h"

namespace BingleImmersiveImpact {
	float speedValues[ConfigType::EndOfEnumMarker];
	bool customizedL = false;
	bool customizedR = false;
	//Since BSFixedStrings can't be compared with char types, we declare them.
	BSFixedString s_pre("preHitFrame");
	BSFixedString s_sw("weaponSwing");
	BSFixedString s_lsw("weaponLeftSwing");
	BSFixedString s_post("AttackWinStart");
	BSFixedString s_lpost("AttackWinStartLeft");
	BSFixedString s_end("AttackWinEnd");
	BSFixedString s_lend("AttackWinEndLeft");

	void ModifyAttackSpeedByTypes(TESObjectWEAP* wep, int weptype, bool right) {
		//If the weapon is 2 handed
		if (weptype == 5 || weptype == 6) {
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Swing2h] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Swing2h] + speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("2h %f", speedValues[ConfigType::Speed_Swing2h]);
		}

		//If the weapon is 1 handed
		else if (weptype == 1 || weptype == 3 || weptype == 4) {
			if(right)
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Swing1h] + speedValues[ConfigType::Speed_Offset]);
			else
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Swing1h] + speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("1h %f", speedValues[ConfigType::Speed_Swing1h]);
		}

		//If the weapon is a dagger
		else if (weptype == 2) {
			if(right)
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_SwingDag] + speedValues[ConfigType::Speed_Offset]);
			else
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_SwingDag] + speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("dag %f", speedValues[ConfigType::Speed_SwingDag]);
		}

		//Bare hands!
		else if (weptype == 0) {
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_SwingFist] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_SwingFist] + speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("fist %f", speedValues[ConfigType::Speed_SwingFist]);
		}
	}

	//The core funciton of the mod.
	//This function is ran when the papyrus script receives OnAnimationEvent event.
	void EvaluateEvent(StaticFunctionTag *base, BSFixedString event) {
		//_MESSAGE("Func ran");

		//If the event is preHitFrame
		if (event == s_pre) {
			//Calculate Offset values to override any player.modav commands
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", 1);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", 1);
			speedValues[ConfigType::Speed_Offset] = 1 - ActorModifier::GetAV((Actor*)(*g_thePlayer), "WeaponSpeedMult");
			speedValues[ConfigType::Speed_LeftOffset] = 1 - ActorModifier::GetAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult");

			tList<ActiveEffect> *list_ae = ((Actor*)(*g_thePlayer))->magicTarget.GetActiveEffects();
			tList<ActiveEffect>::Iterator it = list_ae->Begin();
			while (!it.End()) {
				ActiveEffect* ae = it.Get();
				//If the player has Elemental Fury applied
				if (ae->item->formID == 181653) {
					speedValues[ConfigType::Speed_Offset] += ae->magnitude - 1;
					speedValues[ConfigType::Speed_LeftOffset] += ae->magnitude - 1;
					break;
				}
				++it;
			}
			//Tried OnActorAction, but it sucks. I figured this way is more reliable.
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), 1);
			ActorModifier::RestrainView((Actor*)(*g_thePlayer), 1);			

			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Pre] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Pre] + speedValues[ConfigType::Speed_LeftOffset]);


			float minRange = max((*g_thePlayer)->race->data.handReach * 1.1f - 10.0f, 75.0f);
			float maxRange = (*g_thePlayer)->race->data.handReach * 1.1f * 4.0f;
			ActorModifier::LockAim(minRange, maxRange);
		} 
		//If the event is weaponSwing
		else if (event == s_sw) {
			if (customizedR) {
				ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_CustomR_Swing] + speedValues[ConfigType::Speed_Offset]);
			}
			else if (papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1)) {
				TESObjectWEAP* wep = ((TESObjectWEAP*)(papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1)));
				int weptype = wep->type();
				ModifyAttackSpeedByTypes(wep, weptype, true);
			}
		}
		//If the event is weaponLeftSwing
		else if (event == s_lsw) {
			if (customizedL) {
				ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_CustomL_Swing] + speedValues[ConfigType::Speed_LeftOffset]);
			}
			else if (papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0)) {
				TESObjectWEAP* wep = ((TESObjectWEAP*)(papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0)));
				int weptype = wep->type();
				ModifyAttackSpeedByTypes(wep, weptype, false);
			}

		}
		//If the event is AttackWinStart or AttackWinStartLeft
		else if (event == s_post || event == s_lpost) {
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), 0);

			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Post] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Post] + speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("post %f", speedValues[ConfigType::Speed_Post]);

			ActorModifier::UnlockAim();
		}
		//If the event is AttackWinEnd or AttackWinEndLeft
		else if (event == s_end || event == s_lend) {
			ActorModifier::RestrainView((Actor*)(*g_thePlayer), 0);
		}
	}

	//Functions below are used to register for papyrus events.
	//This function registers for OnFistRequest event on papyrus
	void RegisterForFistRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for fist request event.");
		g_fistRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	//This function registers for OnInitializeRequested event on papyrus
	void RegisterForInitRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for init request event.");
		g_initRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
		BingleEventInvoker::InitializeRequest();
	}

	//These functions below are for debugging purposes.
	//Especially, notification request and messagebox request might come in handy when you need a real-time, visualized debugging functions.
	//This function registers for OnNotificationRequest event on papyrus
	void RegisterForNotificationRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for notifcation request event.");
		g_notiRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	//This function registers for OnMessageBoxRequest event on papyrus
	void RegisterForMessageBoxRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for messagebox request event.");
		g_msgboxRequestRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void UpdateConfig(StaticFunctionTag *base, UInt32 formId, UInt32 configtype, float v) {
		if (configtype < ConfigType::RestrainMovement) {
			speedValues[configtype] = v;
			if (configtype == ConfigType::Speed_CustomL_Swing) {
				SetCustomized(0, true);
				if (papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1) == papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0)) {
					speedValues[ConfigType::Speed_CustomR_Swing] = v;
					SetCustomized(1, true);
					BingleEventInvoker::SyncConfig(ConfigType::Speed_CustomR_Swing, v);
				}
			}
			if (configtype == ConfigType::Speed_CustomR_Swing) {
				SetCustomized(1, true);
				if (papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1) == papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0)) {
					speedValues[ConfigType::Speed_CustomL_Swing] = v;
					SetCustomized(0, true);
					BingleEventInvoker::SyncConfig(ConfigType::Speed_CustomL_Swing, v);
				}
			}
		}
		else if (configtype == ConfigType::RestrainMovement) {
			ActorModifier::EnableRestraint(v);
		}
		else if (configtype == ConfigType::AimHelper) {
			ActorModifier::EnableAimHelper(v);
		}
		ConfigHandler::SetConfig(formId, (ConfigType)configtype, v);
	}

	void SaveConfig(StaticFunctionTag *base) {
		ConfigHandler::SaveConfig();
	}

	void UpdateSaveConfig(StaticFunctionTag *base, UInt32 formId, UInt32 configtype, float v) {
		UpdateConfig(base, formId, configtype, v);
		SaveConfig(base);
	}

	void UpdateFromConfig(ConfigType c, float v) {
		speedValues[c] = v;
	}

	void SetCustomized(int slot, bool b) {
		if (slot == 0)
			customizedL = b;
		else
			customizedR = b;
	}

	float GetDefault(ConfigType c) {
		switch (c) {
			case ConfigType::Speed_Pre:
				return 0.8f;
			case ConfigType::Speed_Swing1h:
				return 1.4f;
			case ConfigType::Speed_Swing2h:
				return 1.75f;
			case ConfigType::Speed_SwingDag:
				return 1.0f;
			case ConfigType::Speed_SwingFist:
				return 1.25f;
			case ConfigType::Speed_Post:
				return 0.9f;
			case ConfigType::RestrainMovement:
				return 0;
			case ConfigType::AimHelper:
				return 0;
		}
	}

	void RegisterForConfigRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for config event.");
		g_configRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}

	void RegisterForTranslateToRequest(BGSRefAlias* thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for translateto event.");
		g_translateToRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}
}


bool Papyrus::RegisterFuncs(VMClassRegistry * registry) {
	//Check out SKSE documents for information on these.
	//In a nutshell, they create native functions for your papyrus script.
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("EvaluateEvent", "BingleImmersiveFeedback", BingleImmersiveImpact::EvaluateEvent, registry));
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
		new NativeFunction3 <StaticFunctionTag, void, UInt32, UInt32, float>("UpdateConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::UpdateConfig, registry));
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, UInt32, UInt32, float>("UpdateSaveConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::UpdateSaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("SaveConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::SaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForConfigRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForConfigRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForConfigRequest", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForTranslateToRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForTranslateToRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForTranslateToRequest", VMClassRegistry::kFunctionFlag_NoWait);
	return true;
}
