#include "ConfigHandler.h"
#include "Papyrus.h"
#include "SKSE/PapyrusEvents.h"
#include <sys/stat.h>

#define CTYPE(name) BingleImmersiveImpact::ConfigType::name
#define CNAME(ctype) BingleImmersiveImpact::ConfigTypeNames[ctype]
#define UNNAMED "Unnamed Weapon";

CSimpleIniA ConfigHandler::ini(true, false, false);
ConfigHandler *ConfigHandler::instance = nullptr;
const char *ConfigHandler::filepath = "Data\\SKSE\\Plugins\\ImmersiveImpact.ini";

bool FileExists(const char* path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

ConfigHandler::ConfigHandler() {
	if (instance)
		delete(instance);
	instance = this;
	if (!FileExists(filepath)) {
		char f2c[128];
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(Speed_Pre)));
		ini.SetValue("General", "Pre", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(Speed_Swing1h)));
		ini.SetValue("General", "Swing1h", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(Speed_Swing2h)));
		ini.SetValue("General", "Swing2h", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(Speed_SwingDag)));
		ini.SetValue("General", "SwingDag", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(Speed_SwingFist)));
		ini.SetValue("General", "SwingFist", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(Speed_Post)));
		ini.SetValue("General", "Post", f2c);
		ini.SaveFile(filepath, false);
	}
	SI_Error error = ini.LoadFile(filepath);
	if (error < 0) {
		_MESSAGE("Error loading ini data!");
		return;
	}
}

void ConfigHandler::LoadConfig(const char *type, int weapontype, int slot) {
	if (strcmp(type, "General") == 0) {
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Pre), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Pre)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Swing1h), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Swing1h)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Swing2h), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Swing2h)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_SwingDag), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_SwingDag)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_SwingFist), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_SwingFist)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Post), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Post)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Pre), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Pre)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Swing1h), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Swing1h)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Swing2h), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Swing2h)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_SwingDag), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_SwingDag)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_SwingFist), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_SwingFist)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Post), std::stof(ini.GetValue(type, CNAME(CTYPE(Speed_Post)), NULL, NULL)));
	}
	else{
		const char *_type = type;
		if (strcmp(type, "") == 0)
			_type = UNNAMED;
		_MESSAGE("Config existence check");
		if (Exists(_type)) {
			_MESSAGE("Exists");
			BingleImmersiveImpact::SetCustomized(slot, true);
			if (slot == 0) {
				BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_CustomL_Swing), std::stof(ini.GetValue(_type, CNAME(CTYPE(Speed_CustomL_Swing)), NULL, NULL)));
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomL_Swing), std::stof(ini.GetValue(_type, CNAME(CTYPE(Speed_CustomL_Swing)), NULL, NULL)));
			} else {
				BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(_type, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(_type, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
			}
		} else {
			_MESSAGE("Does not exist");
			float def = 0;
			if (weapontype == 5 || weapontype == 6) {
				def = BingleImmersiveImpact::GetDefault(CTYPE(Speed_Swing2h));
			} else if (weapontype == 1 || weapontype == 3 || weapontype == 4) {
				def = BingleImmersiveImpact::GetDefault(CTYPE(Speed_Swing1h));
			} else if (weapontype == 2) {
				def = BingleImmersiveImpact::GetDefault(CTYPE(Speed_SwingDag));
			} else if (weapontype == 0) {
				def = BingleImmersiveImpact::GetDefault(CTYPE(Speed_SwingFist));
			}
			if(slot == 0){
				//SetConfig(_type, CTYPE(Speed_CustomL_Swing), def, true);
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomL_Swing), def);
			} else{
				//SetConfig(_type, CTYPE(Speed_CustomR_Swing), def, true);
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomR_Swing), def);
			}
		}
	}
}

void ConfigHandler::SetConfig(const char * type, BingleImmersiveImpact::ConfigType ctype, float v, bool save) {
	char f2c[128];
	sprintf_s(f2c, "%f", v);
	const char *_type = type;
	if (strcmp(type, "") == 0)
		_type = UNNAMED;
	ini.SetValue(_type, CNAME(ctype), f2c);
	if (save)
		SaveConfig();
}

void ConfigHandler::SaveConfig() {
	ini.SaveFile(filepath, false);
}

bool ConfigHandler::Exists(const char * type) {
	return (ini.GetSectionSize(type) >= 0);
}
