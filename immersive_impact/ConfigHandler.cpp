#include "ConfigHandler.h"
#include "Papyrus.h"
#include "ActorModifier.h"
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
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(RestrainMovement)));
		ini.SetValue("General", "RestrainMovement", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(AimHelper)));
		ini.SetValue("General", "AimHelper", f2c);
		sprintf_s(f2c, "%f", BingleImmersiveImpact::GetDefault(CTYPE(ChargeMul)));
		ini.SetValue("General", "ChargeMul", f2c);
		ini.SaveFile(filepath, false);
	}
	SI_Error error = ini.LoadFile(filepath);
	if (error < 0) {
		_MESSAGE("Error loading ini data!");
		return;
	}
}

void ConfigHandler::LoadConfig(UInt32 formId, int weapontype, int slot) {
	SI_Error error = ini.LoadFile(filepath);
	if (error < 0) {
		_MESSAGE("Error loading ini data!");
		return;
	}
	char formIdstr[32];
	sprintf_s(formIdstr, "%lu", formId);
	_MESSAGE("Loaindg ini");
	if (formId == 0) {
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Pre), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Pre)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Swing1h), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Swing1h)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Swing2h), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Swing2h)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_SwingDag), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_SwingDag)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_SwingFist), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_SwingFist)), NULL, NULL)));
		BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_Post), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Post)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Pre), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Pre)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Swing1h), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Swing1h)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Swing2h), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Swing2h)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_SwingDag), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_SwingDag)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_SwingFist), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_SwingFist)), NULL, NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(Speed_Post), std::stof(ini.GetValue("General", CNAME(CTYPE(Speed_Post)), NULL, NULL)));

		if (std::stof(ini.GetValue("General", CNAME(CTYPE(RestrainMovement)), "0", NULL)) == 1)
			ActorModifier::EnableRestraint(true);
		if (std::stof(ini.GetValue("General", CNAME(CTYPE(AimHelper)), "0", NULL)) == 1)
			ActorModifier::EnableAimHelper(true);
		BingleEventInvoker::SyncConfig(CTYPE(RestrainMovement), std::stof(ini.GetValue("General", CNAME(CTYPE(RestrainMovement)), "0", NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(AimHelper), std::stof(ini.GetValue("General", CNAME(CTYPE(AimHelper)), "0", NULL)));
		BingleEventInvoker::SyncConfig(CTYPE(ChargeMul), std::stof(ini.GetValue("General", CNAME(CTYPE(ChargeMul)), "1.0", NULL)));
	}
	else{
		_MESSAGE("Config existence check for %s", formIdstr);
		if (Exists(formIdstr)) {
			_MESSAGE("Exists");
			BingleImmersiveImpact::SetCustomized(slot, true);
			if (slot == 0) {
				BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_CustomL_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomL_Swing)), NULL, NULL)));
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomL_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomL_Swing)), NULL, NULL)));
			} else {
				BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
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

void ConfigHandler::SetConfig(UInt32 formId, BingleImmersiveImpact::ConfigType ctype, float v, bool save) {
	char f2c[128];
	sprintf_s(f2c, "%f", v);
	char formIdstr[32];
	if (formId != 0)
		sprintf_s(formIdstr, "%lu", formId);
	else
		sprintf_s(formIdstr, "%s", "General");
	ini.SetValue(formIdstr, CNAME(ctype), f2c);
	if (save)
		SaveConfig();
}

void ConfigHandler::SaveConfig() {
	ini.SaveFile(filepath, false);
}

bool ConfigHandler::Exists(const char * type) {
	return (ini.GetSectionSize(type) >= 0);
}
