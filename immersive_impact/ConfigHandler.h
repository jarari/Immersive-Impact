#pragma once
#include "simpleini-4.17\SimpleIni.h"
#include "Papyrus.h"

class ConfigHandler {
	static CSimpleIniA ini;
	static ConfigHandler *instance;
	static const char* filepath;
public:
	ConfigHandler();
	static void LoadConfig(const char *type, int weapontype = 0, int slot = 0);
	static void SetConfig(const char *type, BingleImmersiveImpact::ConfigType ctype, float v, bool save = false);
	static void SaveConfig();
	static bool Exists(const char *type);
};