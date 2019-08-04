#pragma once
#include "simpleini-4.17\SimpleIni.h"
#include "Papyrus.h"

class ConfigHandler {
	static CSimpleIniA ini;
	static ConfigHandler *instance;
	static const char* filepath;
public:
	ConfigHandler();
	static void LoadConfig(UInt32 formId, int weapontype = 0, int slot = 0);
	static void SetConfig(UInt32 formId, BingleImmersiveImpact::ConfigType ctype, float v, bool save = false);
	static void SaveConfig();
	static bool Exists(const char *type);
};