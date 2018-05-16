#include "Papyrus.h"
#include "SKSE/PapyrusNativeFunctions.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "SKSE/PapyrusEvents.h"
#include "ActorModifier.h"
#include "ConfigHandler.h"

namespace BingleImmersiveImpact {
	const char *ConfigTypeNames[ConfigType::EndOfEnumMarker] = {
		"Offset",
		"LeftOffset",
		"Pre",
		"Swing1h",
		"Swing2h",
		"SwingDag",
		"SwingFist",
		"Post",
		"Custom_Swing",
		"Custom_Swing"
	};
	float speedValues[ConfigType::EndOfEnumMarker];
	bool customizedL = false;
	bool customizedR = false;
	//The core funciton of the mod.
	//This function is ran when the papyrus script receives OnAnimationEvent event.
	static TESForm *unarmed;
	void EvaluateTypes(StaticFunctionTag *base, BSFixedString event) {
		//Since BSFixedStrings can't be compared with char types, we declare them.
		BSFixedString s_pre("preHitFrame");
		BSFixedString s_sw("weaponSwing");
		BSFixedString s_lsw("weaponLeftSwing");
		BSFixedString s_post("AttackWinStart");
		//_MESSAGE("Func ran");

		//If the event is preHitFrame
		if (event == s_pre) {
			//Tried OnActorAction, but it sucks. I figured this way is more reliable.
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), 1);
			ActorModifier::RestrainView((Actor*)(*g_thePlayer), 1);

			//A cleaner way to modify actor values.
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Pre] - speedValues[ConfigType::Speed_Offset]);
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Pre] - speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("pre %f", speedValues[ConfigType::Speed_Pre]);
			if (abs(ActorModifier::GetAV((Actor*)(*g_thePlayer), "WeaponSpeedMult") - speedValues[ConfigType::Speed_Pre]) > 0.05) {
				speedValues[ConfigType::Speed_Offset] = (ActorModifier::GetAV((Actor*)(*g_thePlayer), "WeaponSpeedMult") - speedValues[ConfigType::Speed_Pre]);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Pre] - speedValues[ConfigType::Speed_Offset]);
			}
			if (abs(ActorModifier::GetAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult") - speedValues[ConfigType::Speed_Pre]) > 0.05) {
				speedValues[ConfigType::Speed_LeftOffset] = (ActorModifier::GetAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult") - speedValues[ConfigType::Speed_Pre]);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Pre] - speedValues[ConfigType::Speed_LeftOffset]);
			}
		//If the event is weaponSwing or weaponLeftSwing
		} else if (event == s_sw || event == s_lsw) {
			int weptype = 0;
			//We have to get the weapon for each hand depending on the event.
			//Make sure the weapon "exists" or it will cause CTD.
			if (event == s_sw && papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1))
				weptype = ((TESObjectWEAP*)(papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 1)))->type();
			else if(event == s_lsw && papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0))
				weptype = ((TESObjectWEAP*)(papyrusActor::GetEquippedObject((Actor*)(*g_thePlayer), 0)))->type();
			
			if (customizedL) {
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_CustomL_Swing] - speedValues[ConfigType::Speed_LeftOffset]);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_CustomR_Swing] - speedValues[ConfigType::Speed_Offset]);
			}
			else if (customizedR) {
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_CustomL_Swing] - speedValues[ConfigType::Speed_LeftOffset]);
				ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_CustomR_Swing] - speedValues[ConfigType::Speed_Offset]);
			}
			else {
				//If the weapon is 2 handed
				if (weptype == 5 || weptype == 6) {
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Swing2h] - speedValues[ConfigType::Speed_Offset]);
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Swing2h] - speedValues[ConfigType::Speed_LeftOffset]);
					//_MESSAGE("2h %f", speedValues[ConfigType::Speed_Swing2h]);

					//If the weapon is 1 handed
				} else if (weptype == 1 || weptype == 3 || weptype == 4) {
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Swing1h] - speedValues[ConfigType::Speed_Offset]);
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Swing1h] - speedValues[ConfigType::Speed_LeftOffset]);
					//_MESSAGE("1h %f", speedValues[ConfigType::Speed_Swing1h]);

					//If the weapon is a dagger
				} else if (weptype == 2) {
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_SwingDag] - speedValues[ConfigType::Speed_Offset]);
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_SwingDag] - speedValues[ConfigType::Speed_LeftOffset]);
					//_MESSAGE("dag %f", speedValues[ConfigType::Speed_SwingDag]);

					//Bare hands!
				} else if (weptype == 0) {
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_SwingFist] - speedValues[ConfigType::Speed_Offset]);
					ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_SwingFist] - speedValues[ConfigType::Speed_LeftOffset]);
					//_MESSAGE("fist %f", speedValues[ConfigType::Speed_SwingFist]);
				}
			}
		//If the event is AttackWinStart
		} else if (event == s_post) {
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), 0);

			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Post] - speedValues[ConfigType::Speed_Offset]);
			ActorModifier::ModifyAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Post] - speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("post %f", speedValues[ConfigType::Speed_Post]);

			speedValues[ConfigType::Speed_Offset] = 0;
			speedValues[ConfigType::Speed_LeftOffset] = 0;
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

	//Not used anymore.
	void RestrainMovement(StaticFunctionTag *base, Actor* a, UInt32 i) {
		ActorModifier::RestrainMovement(a, i);
	}

	void EnableRestrainMovement(StaticFunctionTag *base) {
		ActorModifier::EnableRestraint(true);
		_MESSAGE("Restraint enabled.");
	}

	void DisableRestrainMovement(StaticFunctionTag *base) {
		ActorModifier::EnableRestraint(false);
	}

	void UpdateConfig(StaticFunctionTag *base, BSFixedString type, UInt32 configtype, float v) {
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
		ConfigHandler::SetConfig(type.data, (ConfigType)configtype, v);
	}

	void SaveConfig(StaticFunctionTag *base) {
		ConfigHandler::SaveConfig();
	}

	void UpdateSaveConfig(StaticFunctionTag *base, BSFixedString type, UInt32 configtype, float v) {
		UpdateConfig(base, type, configtype, v);
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
			break;
		case ConfigType::Speed_Swing1h:
			return 1.4f;
			break;
		case ConfigType::Speed_Swing2h:
			return 1.75f;
			break;
		case ConfigType::Speed_SwingDag:
			return 1.0f;
			break;
		case ConfigType::Speed_SwingFist:
			return 1.25f;
			break;
		case ConfigType::Speed_Post:
			return 0.9f;
			break;
		}
	}

	void RegisterForConfigRequest(BGSRefAlias * thisForm) {
		if (!thisForm)
			return;
		_MESSAGE("Registering for config event.");
		g_configRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
	}
}


bool Papyrus::RegisterFuncs(VMClassRegistry * registry) {
	//Check out SKSE documents for information on these.
	//In a nutshell, they create native functions for your papyrus script.
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("EvaluateTypes", "BingleImmersiveFeedback", BingleImmersiveImpact::EvaluateTypes, registry));
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
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, UInt32, float>("UpdateConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::UpdateConfig, registry));
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, UInt32, float>("UpdateSaveConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::UpdateSaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("SaveConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::SaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterForConfigRequest", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterForConfigRequest, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterForConfigRequest", VMClassRegistry::kFunctionFlag_NoWait);


	//Not used anymore.
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, Actor*, UInt32>("RestrainMovement", "BingleImmersiveFeedback", BingleImmersiveImpact::RestrainMovement, registry));
	
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("EnableRestrainMovement", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::EnableRestrainMovement, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("DisableRestrainMovement", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::DisableRestrainMovement, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("EnableRestrainMovement", "BingleImmersiveFeedback", BingleImmersiveImpact::EnableRestrainMovement, registry));
	return true;
}
