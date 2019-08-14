#pragma once
#include <SKSE/PapyrusEvents.h>
extern RegistrationSetHolder<NullParameters>						g_mainScriptRegs;
class BingleEventInvoker {
public:
	static void EquipFist(Actor* a);
	static void EquipItem(Actor* a, TESForm* wep, UInt32 choice);
	static void InitializeRequest();
	static void SendNotification(BSFixedString msg);
	static void ShowMessageBox(BSFixedString msg);
	static void SyncConfig(UInt32 ctype, float v);
	static void TranslateToTarget(Actor* a);
	static void TranslateTo(float x, float y, float z, float vel);
	static void StopTranslation();
	static void PlayDeflectSound(TESObjectREFR* obj);
};