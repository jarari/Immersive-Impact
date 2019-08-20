#include "ConfigHandler.h"
#include "Papyrus.h"
#include "ActorModifier.h"
#include "BingleEventInvoker.h"
#include <sys/stat.h>
#include <immersive_impact\HitFeedback.h>

const char* ConfigTypeNames[ConfigType::EndOfEnumMarker] = {
	"Offset",
	"LeftOffset",
	"Pre",
	"Swing1h",
	"Swing2h",
	"SwingDag",
	"SwingFist",
	"Post",
	"Custom_Swing",
	"Custom_Swing",
	"RestrainMovement",
	"AimHelper",
	"ActivationRangeMul",
	"HitFeedback",
	"SpeedAdjustment",
	"DeflectChanceMul",
	"DeflectChanceMax",
	"StaggerResetTime",
	"StaggerLimit",
	"StaggerDamageMax",
	"StaggerAny",
	"ChargeDistMax",
	"AimCompensationStrength",
	"AlwaysChargeIn",
	"ChargeVelocity"
};
float configValues[ConfigType::EndOfEnumMarker];

#define CTYPE(name) ConfigType::name
#define CNAME(ctype) ConfigTypeNames[ctype]
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
		sprintf_s(f2c, "%f", GetDefault(CTYPE(ActivationRangeMul)));
		ini.SetValue("General", "ActivationRangeMul", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(EnableHitFeedback)));
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
		sprintf_s(f2c, "%f", GetDefault(CTYPE(ChargeDistMax)));
		ini.SetValue("General", "ChargeDistMax", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(AimCompensationStrength)));
		ini.SetValue("General", "AimCompensationStrength", f2c);
		sprintf_s(f2c, "%f", GetDefault(CTYPE(AlwaysChargeIn)));
		ini.SetValue("General", "AlwaysChargeIn", f2c);
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

		type = CTYPE(ActivationRangeMul);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(EnableHitFeedback);
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

		type = CTYPE(ChargeDistMax);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(AimCompensationStrength);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(AlwaysChargeIn);
		snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
		val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
		UpdateFromConfig(type, val);
		BingleEventInvoker::SyncConfig(type, val);

		type = CTYPE(ChargeVelocity);
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
				UpdateFromConfig(CTYPE(Speed_CustomR_Swing), std::stof(ini.GetValue(formIdstr, CNAME(CTYPE(Speed_CustomR_Swing)), NULL, NULL)));
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

void ConfigHandler::SetConfig(UInt32 formId, ConfigType ctype, float v, bool save) {
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

float ConfigHandler::GetDefault(ConfigType c) {
	switch (c) {
		case ConfigType::Speed_Pre:
			return 0.6f;
		case ConfigType::Speed_Swing1h:
			return 1.5f;
		case ConfigType::Speed_Swing2h:
			return 1.8f;
		case ConfigType::Speed_SwingDag:
			return 1.1f;
		case ConfigType::Speed_SwingFist:
			return 1.3f;
		case ConfigType::Speed_Post:
			return 1.25f;
		case ConfigType::RestrainMovement:
			return 0;
		case ConfigType::AimHelper:
			return 0;
		case ConfigType::ActivationRangeMul:
			return 1.0f;
		case ConfigType::EnableHitFeedback:
			return 0;
		case ConfigType::SpeedAdjustment:
			return 1;
		case ConfigType::DeflectChanceMul:
			return 0.1f;
		case ConfigType::DeflectChanceMax:
			return 50.0f;
		case ConfigType::StaggerResetTime:
			return 2.0f;
		case ConfigType::StaggerLimit:
			return 3.0f;
		case ConfigType::StaggerDamageMax:
			return 400.0f;
		case ConfigType::StaggerAny:
			return 0;
		case ConfigType::ChargeDistMax:
			return 300.0f;
		case ConfigType::AimCompensationStrength:
			return 1.0f;
		case ConfigType::AlwaysChargeIn:
			return 0;
		case ConfigType::ChargeVelocity:
			return 100.0f;
	}
	return 0.0f;
}

void ConfigHandler::UpdateFromConfig(ConfigType configtype, float v) {
	configValues[configtype] = v;
}