#include "ConfigHandler.h"
#include "Papyrus.h"
#include "ActorModifier.h"
#include "BingleEventInvoker.h"
#include <sys/stat.h>
#include <immersive_impact\HitFeedback.h>

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

using namespace BingleImmersiveImpact;
ConfigHandler::ConfigHandler() {
	if (instance)
		delete(instance);
	instance = this;
	if (!FileExists(filepath)) {
		char f2c[128];
		sprintf_s(f2c, "%f", GetDefault(CTYPE(Speed_Pre)));
		ini.SetValue("General", "Pre", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(Speed_Swing1h)));
		ini.SetValue("General", "Swing1h", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(Speed_Swing2h)));
		ini.SetValue("General", "Swing2h", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(Speed_SwingDag)));
		ini.SetValue("General", "SwingDag", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(Speed_SwingFist)));
		ini.SetValue("General", "SwingFist", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(Speed_Post)));
		ini.SetValue("General", "Post", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(RestrainMovement)));
		ini.SetValue("General", "RestrainMovement", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(AimHelper)));
		ini.SetValue("General", "AimHelper", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(ChargeMul)));
		ini.SetValue("General", "ChargeMul", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(HitFeedback)));
		ini.SetValue("General", "HitFeedback", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(SpeedAdjustment)));
		ini.SetValue("General", "SpeedAdjustment", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(DeflectChanceMul)));
		ini.SetValue("General", "DeflectChanceMul", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(DeflectChanceMax)));
		ini.SetValue("General", "DeflectChanceMax", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(StaggerResetTime)));
		ini.SetValue("General", "StaggerResetTime", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(StaggerLimit)));
		ini.SetValue("General", "StaggerLimit", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(StaggerDamageMax)));
		ini.SetValue("General", "StaggerDamageMax", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(StaggerAny)));
		ini.SetValue("General", "StaggerAny", f2c);
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
	if (formId == 0) {
		ConfigType type = ConfigType::Speed_Swing2h;
		char defaultbuf[8];
		float val = 0;

		type = CTYPE(SpeedAdjustment);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		if (val == 1)
			HitFeedback::EnableFeedback(true);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(Speed_Pre);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(Speed_Swing1h);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(Speed_Swing2h);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(Speed_SwingDag);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(Speed_SwingFist);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(Speed_Post);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(RestrainMovement);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		if (val == 1)
			ActorModifier::EnableRestraint(true);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(AimHelper);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		if (val == 1)
			ActorModifier::EnableAimHelper(true);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(ChargeMul);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(HitFeedback);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		if (val == 1)
			HitFeedback::EnableFeedback(true);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(SpeedAdjustment);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		if (val == 1)
			EnableSpeedAdjustment(true);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(DeflectChanceMul);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(DeflectChanceMax);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(StaggerResetTime);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(StaggerLimit);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(StaggerDamageMax);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(StaggerAny);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);
	}
	else{
		if (Exists(formIdstr)) {
			SetCustomized(slot, true);
			if (slot == 0) {
				UpdateFromConfig(CTYPE(Speed_CustomL_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomL_Swing)), NULL, NULL)));
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomL_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomL_Swing)), NULL, NULL)));
			} else {
				BingleImmersiveImpact::UpdateFromConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
				BingleEventInvoker::SyncConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
			}
		} else {
			float def = 0;
			if (weapontype == 5 || weapontype == 6) {
				def = GetDefault(CTYPE(Speed_Swing2h));
			} else if (weapontype == 1 || weapontype == 3 || weapontype == 4) {
				def = GetDefault(CTYPE(Speed_Swing1h));
			} else if (weapontype == 2) {
				def = GetDefault(CTYPE(Speed_SwingDag));
			} else if (weapontype == 0) {
				def = GetDefault(CTYPE(Speed_SwingFist));
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
