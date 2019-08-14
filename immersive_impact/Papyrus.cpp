#include "Papyrus.h"
#include "SKSE/PapyrusNativeFunctions.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/GameObjects.h"
#include "SKSE/GameReferences.h"
#include "BingleEventInvoker.h"
#include "SKSE/GameData.h"
#include "ActorModifier.h"
#include "ConfigHandler.h"
#include "EquipWatcher.h"

namespace BingleImmersiveImpact {
	float speedValues[ConfigType::EndOfEnumMarker];
	float chargeMul = 1.0f;
	bool customizedL = false;
	bool customizedR = false;
	//Since BSFixedStrings can't be compared with char types, we declare them.
	BSFixedString s_as("attackStop");
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

	void SetCustomized(int slot, bool b) {
		if (slot == 0)
			customizedL = b;
		else
			customizedR = b;
	}

	float GetDefault(ConfigType c) {
		switch (c) {
			case ConfigType::Speed_Pre:
				return 0.6f;
			case ConfigType::Speed_Swing1h:
				return 1.5f;
			case ConfigType::Speed_Swing2h:
				return 1.8f;
			case ConfigType::Speed_SwingDag:
				return 1.1f;
			case ConfigType::Speed_SwingFist:
				return 1.7f;
			case ConfigType::Speed_Post:
				return 1.25f;
			case ConfigType::RestrainMovement:
				return 0;
			case ConfigType::AimHelper:
				return 0;
			case ConfigType::ChargeMul:
				return 1.0f;
			case ConfigType::HitFeedback:
				return 0;
		}
	}


#pragma region PAPYRUS_FUNCTIONS

	//The core funciton of the mod.
	//This function is ran when the papyrus script receives OnAnimationEvent event.
	void EvaluateEvent(StaticFunctionTag* base, BSFixedString event) {
		//_MESSAGE("%s", event);

		if (event == s_as) {
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Pre] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Pre] + speedValues[ConfigType::Speed_LeftOffset]);
		}

		//If the event is preHitFrame
		else if (event == s_pre) {
			//Calculate Offset values to override any player.modav commands
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", 1);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", 1);
			speedValues[ConfigType::Speed_Offset] = 1 - ActorModifier::GetAV((Actor*)(*g_thePlayer), "WeaponSpeedMult");
			speedValues[ConfigType::Speed_LeftOffset] = 1 - ActorModifier::GetAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult");

			tList<ActiveEffect>* list_ae = ((Actor*)(*g_thePlayer))->magicTarget.GetActiveEffects();
			tList<ActiveEffect>::Iterator it = list_ae->Begin();
			while (!it.End()) {
				ActiveEffect* ae = it.Get();
				//Instead of looking for specific for ids, look for what actorvalue does this ActiveEffect change.
				if (ae->actorValue == LookupActorValueByName("WeaponSpeedMult")) {
					speedValues[ConfigType::Speed_Offset] += ae->magnitude - 1;
					speedValues[ConfigType::Speed_LeftOffset] += ae->magnitude - 1;
				}
				++it;
			}
			//Tried OnActorAction, but it sucks. I figured this way is more reliable.
			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), true);
			ActorModifier::RestrainView((Actor*)(*g_thePlayer), false);

			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Pre] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Pre] + speedValues[ConfigType::Speed_LeftOffset]);

			float wepReach = EquipWatcher::isTwoHanded ? ((TESObjectWEAP*)(Actor*)(*g_thePlayer)->GetEquippedObject(false))->reach() : 1.0f;
			float minRange = max((*g_thePlayer)->race->data.handReach, 60.0f);
			float maxRange = (*g_thePlayer)->race->data.handReach * 3.0f * chargeMul * wepReach;
			ActorModifier::LockAim(minRange, maxRange);
		}

		else if (event == s_sw || event == s_lsw) {
			ActorModifier::RestrainView((Actor*)(*g_thePlayer), true);
			//If the event is weaponSwing
			if (event == s_sw) {
				if (customizedR) {
					ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_CustomR_Swing] + speedValues[ConfigType::Speed_Offset]);
				}
				else if ((Actor*)(*g_thePlayer)->GetEquippedObject(false)) {
					TESObjectWEAP* wep = ((TESObjectWEAP*)(Actor*)(*g_thePlayer)->GetEquippedObject(false));
					int weptype = wep->type();
					ModifyAttackSpeedByTypes(wep, weptype, true);
				}
			}
			//If the event is weaponLeftSwing
			else if (event == s_lsw) {
				if (customizedL) {
					ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_CustomL_Swing] + speedValues[ConfigType::Speed_LeftOffset]);
				}
				else if ((Actor*)(*g_thePlayer)->GetEquippedObject(true)) {
					TESObjectWEAP* wep = ((TESObjectWEAP*)(Actor*)(*g_thePlayer)->GetEquippedObject(true));
					int weptype = wep->type();
					ModifyAttackSpeedByTypes(wep, weptype, false);
				}
			}
		}

		//If the event is AttackWinStart or AttackWinStartLeft
		else if (event == s_post || event == s_lpost) {
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "WeaponSpeedMult", speedValues[ConfigType::Speed_Post] + speedValues[ConfigType::Speed_Offset]);
			ActorModifier::SetCurrentAV((Actor*)(*g_thePlayer), "LeftWeaponSpeedMult", speedValues[ConfigType::Speed_Post] + speedValues[ConfigType::Speed_LeftOffset]);
			//_MESSAGE("post %f", speedValues[ConfigType::Speed_Post]);

			ActorModifier::RestrainMovement((Actor*)(*g_thePlayer), false);
			ActorModifier::UnlockAim();
		}
		//If the event is AttackWinEnd or AttackWinEndLeft
		else if (event == s_end || event == s_lend) {
			//ActorModifier::RestrainView((Actor*)(*g_thePlayer), false);
		}
	}

	//This function registers the main script to SKSE.
	void RegisterMainScript(BGSRefAlias* thisForm) {
		if (!thisForm)
			return;
		g_mainScriptRegs.Register<BGSRefAlias>(kFormType_ReferenceAlias, thisForm);
		BingleEventInvoker::InitializeRequest();
	}

	void UpdateConfig(StaticFunctionTag* base, UInt32 formId, UInt32 configtype, float v) {
		if (configtype < ConfigType::RestrainMovement) {
			speedValues[configtype] = v;
			if (configtype == ConfigType::Speed_CustomL_Swing) {
				SetCustomized(0, true);
				if ((Actor*)(*g_thePlayer)->GetEquippedObject(false) == (Actor*)(*g_thePlayer)->GetEquippedObject(true)) {
					speedValues[ConfigType::Speed_CustomR_Swing] = v;
					SetCustomized(1, true);
					BingleEventInvoker::SyncConfig(ConfigType::Speed_CustomR_Swing, v);
				}
			}
			if (configtype == ConfigType::Speed_CustomR_Swing) {
				SetCustomized(1, true);
				if ((Actor*)(*g_thePlayer)->GetEquippedObject(false) == (Actor*)(*g_thePlayer)->GetEquippedObject(true)) {
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
		else if (configtype == ConfigType::ChargeMul) {
			chargeMul = v;
		}
		ConfigHandler::SetConfig(formId, (ConfigType)configtype, v);
	}

	void SaveConfig(StaticFunctionTag* base) {
		ConfigHandler::SaveConfig();
	}

	void UpdateSaveConfig(StaticFunctionTag* base, UInt32 formId, UInt32 configtype, float v) {
		UpdateConfig(base, formId, configtype, v);
		SaveConfig(base);
	}

	void UpdateFromConfig(ConfigType c, float v) {
		speedValues[c] = v;
	}

#pragma endregion
}


bool Papyrus::RegisterFuncs(VMClassRegistry * registry) {
	//Check out SKSE documents for information on these.
	//In a nutshell, they create native functions for your papyrus script.
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("EvaluateEvent", "BingleImmersiveFeedback", BingleImmersiveImpact::EvaluateEvent, registry));
	registry->RegisterFunction(
		new NativeFunction0 <BGSRefAlias, void>("RegisterMainScript", "BingleImmersiveFeedback", BingleImmersiveImpact::RegisterMainScript, registry));
	registry->SetFunctionFlags("BingleImmersiveFeedback", "RegisterMainScript", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, UInt32, UInt32, float>("UpdateConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::UpdateConfig, registry));
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, UInt32, UInt32, float>("UpdateSaveConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::UpdateSaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("SaveConfig", "BingleImmersiveFeedbackMCM", BingleImmersiveImpact::SaveConfig, registry));
	return true;
}
