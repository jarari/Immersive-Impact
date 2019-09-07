#include "CameraController.h"
#include "ConfigHandler.h"
#include "MenuCloseWatcher.h"
#include "NiNodeUpdateWatcher.h"
#include "Papyrus.h"
#include <skse/PluginAPI.h>		// super
#include <skse/skse_version.h>	// What version of SKSE is running?
#include <shlobj.h>				// CSIDL_MYCODUMENTS
#include <string>
#include <thread>
using std::string;
static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
std::string PLUGIN_NAME = "Next-Gen Camera";
static SKSEPapyrusInterface* g_papyrus = NULL;

extern "C" {

	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info) {	// Called by SKSE to learn about this plugin and check that it's safe to load it
		char logfilename[128];
		sprintf_s(logfilename, "\\My Games\\Skyrim\\SKSE\\%s.log", PLUGIN_NAME.c_str());
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, logfilename);
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME.c_str();
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if (skse->runtimeVersion != RUNTIME_VERSION_1_9_32_0)
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
	bool SKSEPlugin_Load(const SKSEInterface* skse) {	// Called by SKSE to load this plugin
		_MESSAGE((PLUGIN_NAME + ((string)" loaded")).c_str());

		SKSEMessagingInterface* skseMsg = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);

		g_papyrus = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		g_papyrus->Register(Papyrus::RegisterFuncs);
		MenuCloseWatcher::InitHook();
		NiNodeUpdateWatcher::InitHook(skseMsg);
		ConfigHandler::InitHandler();
		ConfigHandler::LoadConfig();
		skseMsg->RegisterListener(skse->GetPluginHandle(), "SKSE", [](SKSEMessagingInterface::Message* msg) -> void {
			if (msg->type == SKSEMessagingInterface::kMessage_PreLoadGame) {
				CameraController::hookActive = false;
				CameraController::papyrusActive = false;
			}
			else if (msg->type == SKSEMessagingInterface::kMessage_PostLoadGame) {
				if ((bool)msg->data) {
					CameraController::hookActive = true;
					CameraController::ResetLastDispatcher();
					_MESSAGE("Game loaded successfully.");
				}
			}
			else if (msg->type == SKSEMessagingInterface::kMessage_NewGame) {
				CameraController::papyrusActive = false;
				CameraController::ResetLastDispatcher();
			}
		});

		std::thread cameraThread(CameraController::MainBehavior);
		cameraThread.detach();

		return true;
	}

};