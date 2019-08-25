#pragma once
#include <SKSE/PapyrusEvents.h>

extern RegistrationSetHolder<NullParameters> g_mainScriptRegs;
namespace CustomEvent {
	void SyncConfig(UInt32 type, float val);
	void SyncDefault(UInt32 type, float val);
	void RegisterScript(TESQuest* thisForm);
}

//Register papyrus functions
class VMClassRegistry;
namespace Papyrus {
	bool RegisterFuncs(VMClassRegistry* registry);
};