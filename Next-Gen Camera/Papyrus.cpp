#include "Papyrus.h"
#include "CameraController.h"
#include "ConfigHandler.h"
#include <SKSE\PapyrusNativeFunctions.h>
#include <SKSE\GameForms.h>
#include <SKSE\PapyrusEvents.cpp>

RegistrationSetHolder<NullParameters> g_mainScriptRegs;

void CustomEvent::SyncConfig(UInt32 type, float val) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor2<UInt32, float>(BSFixedString("OnSyncConfig"), type, val)
	);
}

void CustomEvent::SyncDefault(UInt32 type, float val) {
	g_mainScriptRegs.ForEach(
		EventQueueFunctor2<UInt32, float>(BSFixedString("OnSyncDefault"), type, val)
	);
}

#pragma region PAPYRUS_FUNCTIONS

void RegisterScript(TESQuest* thisForm) {
	if (!thisForm)
		return;
	g_mainScriptRegs.Register<TESQuest>(kFormType_Quest, thisForm);
	_MESSAGE("Papyrus registered");
}

void UpdateConfig(StaticFunctionTag* base, UInt32 configtype, float v) {
	configValues[configtype] = v;

	ConfigHandler::SetConfig((ConfigType)configtype, v);
}

void SaveConfig(StaticFunctionTag* base) {
	ConfigHandler::SaveConfig();
}

void UpdateSaveConfig(StaticFunctionTag* base, UInt32 configtype, float v) {
	UpdateConfig(base, configtype, v);
	SaveConfig(base);
}

void ForceSyncConfig(StaticFunctionTag* base) {
	ConfigHandler::LoadConfig();
}
#pragma endregion

bool Papyrus::RegisterFuncs(VMClassRegistry* registry) {
	registry->RegisterFunction(
		new NativeFunction0 <TESQuest, void>("RegisterScript", "BingleNextGenCameraMCM", RegisterScript, registry));
	registry->SetFunctionFlags("BingleNextGenCameraMCM", "RegisterScript", VMClassRegistry::kFunctionFlag_NoWait);
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, UInt32, float>("UpdateConfig", "BingleNextGenCameraMCM", UpdateConfig, registry));
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, UInt32, float>("UpdateSaveConfig", "BingleNextGenCameraMCM", UpdateSaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("SaveConfig", "BingleNextGenCameraMCM", SaveConfig, registry));
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("ForceSyncConfig", "BingleNextGenCameraMCM", ForceSyncConfig, registry));
	return true;
}
