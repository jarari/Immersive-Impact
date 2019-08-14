#include "BingleEventInvoker.h"
#include <SKSE\PapyrusEvents.cpp>
RegistrationSetHolder<NullParameters>						g_mainScriptRegs;

void BingleEventInvoker::EquipFist(Actor* a) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<TESObjectREFR*>(BSFixedString("OnFistRequested"), (TESObjectREFR*)a)
	);
}

void BingleEventInvoker::EquipItem(Actor* a, TESForm* wep, UInt32 choice) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor4<TESObjectREFR*, TESForm*, UInt32, UInt32>(BSFixedString("OnEquipRequested"), (TESObjectREFR*)a, (TESForm*)wep, choice, NULL)
	);
}

void BingleEventInvoker::InitializeRequest() {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<UInt32>(BSFixedString("OnInitializeRequested"), 0)
	);
}

void BingleEventInvoker::SendNotification(BSFixedString msg) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<BSFixedString>(BSFixedString("OnNotificationRequest"), msg)
	);
}

void BingleEventInvoker::ShowMessageBox(BSFixedString msg) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<BSFixedString>(BSFixedString("OnMessageBoxRequest"), msg)
	);
}

void BingleEventInvoker::SyncConfig(UInt32 ctype, float v) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor2<UInt32, float>(BSFixedString("OnSyncConfig"), ctype, v)
	);
}

void BingleEventInvoker::TranslateToTarget(Actor* a) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<TESObjectREFR*>(BSFixedString("OnTranslateToTarget"), (TESObjectREFR*)a)
	);
}

void BingleEventInvoker::TranslateTo(float x, float y, float z, float vel) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor4<float, float, float, float>(BSFixedString("OnTranslateTo"), x, y, z, vel)
	);
}

void BingleEventInvoker::StopTranslation() {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<UInt32>(BSFixedString("OnStopTranslation"), 0)
	);
}

void BingleEventInvoker::PlayDeflectSound(TESObjectREFR* obj) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor1<TESObjectREFR*>(BSFixedString("OnDeflectSoundPlay"), obj)
	);
}