#include "skse/PluginAPI.h"		// super
#include "skse/skse_version.h"	// What version of SKSE is running?
#include <shlobj.h>				// CSIDL_MYCODUMENTS
#include "SKSE/GameEvents.h"

#include "PluginData.h"
#include "Papyrus.h"
#include "EquipWatcher.h"
#include "MenuCloseWatcher.h"
#include "GameLoadWatcher.h"
#include "WeaponSwingWatcher.h"
#include "RaceSwitchWatcher.h"
#include "ConfigHandler.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x8000 ? '1' : '0'), \
  (byte & 0x4000 ? '1' : '0'), \
  (byte & 0x2000 ? '1' : '0'), \
  (byte & 0x1000 ? '1' : '0'), \
  (byte & 0x800 ? '1' : '0'), \
  (byte & 0x400 ? '1' : '0'), \
  (byte & 0x200 ? '1' : '0'), \
  (byte & 0x100 ? '1' : '0'), \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

using namespace PluginData;
using std::string;
static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;

extern "C"	{

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)	{	// Called by SKSE to learn about this plugin and check that it's safe to load it
		char logfilename[128];
		sprintf_s(logfilename, "\\My Games\\Skyrim\\SKSE\\%s.log", PLUGIN_NAME);
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, logfilename);
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		// populate info structure
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME.c_str();
		info->version =		4;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if(skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if(skse->runtimeVersion != RUNTIME_VERSION_1_9_32_0)
		{
			_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

			return false;
		}

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	//On plugin load
	bool SKSEPlugin_Load(const SKSEInterface * skse)	{	// Called by SKSE to load this plugin
		_MESSAGE((PLUGIN_NAME + ((string)" loaded")).c_str());

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);

		//Check if the function registration was a success...
		bool btest = g_papyrus->Register(Papyrus::RegisterFuncs);
		if (btest) {
			//Initialize watchers
			GameLoadWatcher::InitHook();
			MenuCloseWatcher::InitHook();
			EquipWatcher::InitHook();
			WeaponSwingWatcher::InitHook();
			RaceSwitchWatcher::InitHook();
			new ConfigHandler();
			_MESSAGE((PLUGIN_NAME + ((string)" register success")).c_str());
		}

		return true;
	}

};