#include "ConfigHandler.h"
#include "Papyrus.h"
#include <sys/stat.h>

const char* ConfigTypeNames[ConfigType::EndOfEnumMarker] = {
	"iTickRate",
	"fCamSmooth",
	"fIdleOffsetX",
	"fIdleOffsetZ",
	"fCombatOffsetX",
	"fCombatOffsetZ",
	"fMagicOffsetX",
	"fMagicOffsetZ",
	"fBowOffsetX",
	"fBowOffsetZ",
	"fHorseIdleOffsetX",
	"fHorseIdleOffsetZ",
	"fHorseCombatOffsetX",
	"fHorseCombatOffsetZ",
	"fHorseMagicOffsetX",
	"fHorseMagicOffsetZ",
	"fHorseBowOffsetX",
	"fHorseBowOffsetZ",
	"fCamVelocityStrengthX",
	"fSneakIdleOffsetX",
	"fSneakIdleOffsetZ",
	"fSneakCombatOffsetX",
	"fSneakCombatOffsetZ",
	"fSneakMagicOffsetX",
	"fSneakMagicOffsetZ",
	"fSneakBowOffsetX",
	"fSneakBowOffsetZ",
	"fCamVelocityStrengthY",
	"fCamVelocityStrengthZ",
	"iShoulderSwitchKey",
	"iShoulder"
};
float configValues[ConfigType::EndOfEnumMarker];

#define CTYPE(name) ConfigType::name
#define CNAME(ctype) ConfigTypeNames[ctype]

CSimpleIniA ConfigHandler::ini(true, false, false);
ConfigHandler* ConfigHandler::instance = nullptr;
const char* ConfigHandler::filepath = "Data\\SKSE\\Plugins\\Next-Gen Camera.ini";

bool FileExists(const char* path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

void ConfigHandler::InitHandler() {
	if (instance)
		delete(instance);
	instance = new ConfigHandler();
	if (!FileExists(filepath)) {
		char f2c[128];
		ConfigType type;

		type = CTYPE(iTickRate);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fCamSmooth);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fIdleOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fIdleOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fCombatOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fCombatOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fMagicOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fMagicOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fBowOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fBowOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseIdleOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseIdleOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseCombatOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseCombatOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseMagicOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseMagicOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseBowOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fHorseBowOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fCamVelocityStrengthX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fCamVelocityStrengthY);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fCamVelocityStrengthZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakIdleOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakIdleOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakCombatOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakCombatOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakMagicOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakMagicOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakBowOffsetX);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(fSneakBowOffsetZ);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(iShoulderSwitchKey);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);

		type = CTYPE(iShoulder);
		sprintf_s(f2c, "%f", GetDefault(type));
		ini.SetValue("General", CNAME(type), f2c);
		ini.SaveFile(filepath, false);
	}
}

void ConfigHandler::LoadConfig() {
	SI_Error error = ini.LoadFile(filepath);
	if (error < 0) {
		_MESSAGE("Error loading ini data!");
		return;
	}
	ConfigType type;
	char defaultbuf[8];
	float val = 0;

	type = CTYPE(iTickRate);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fCamSmooth);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fIdleOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fIdleOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fCombatOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fCombatOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fMagicOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fMagicOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fBowOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fBowOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseIdleOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseIdleOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseCombatOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseCombatOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseMagicOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseBowOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fHorseBowOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fCamVelocityStrengthX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fCamVelocityStrengthY);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fCamVelocityStrengthZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakIdleOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakIdleOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakCombatOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakCombatOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakMagicOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakBowOffsetX);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(fSneakBowOffsetZ);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(iShoulderSwitchKey);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));

	type = CTYPE(iShoulder);
	snprintf(defaultbuf, sizeof defaultbuf, "%f", GetDefault(type));
	val = std::stof(ini.GetValue("General", CNAME(type), defaultbuf, NULL));
	UpdateFromConfig(type, val);
	CustomEvent::SyncConfig(type, val);
	CustomEvent::SyncDefault(type, GetDefault(type));
}

void ConfigHandler::SetConfig(ConfigType ctype, float v, bool save) {
	char f2c[128];
	sprintf_s(f2c, "%f", v);
	ini.SetValue("General", CNAME(ctype), f2c);
	if (save)
		SaveConfig();
}

void ConfigHandler::SaveConfig() {
	ini.SaveFile(filepath, false);
}

bool ConfigHandler::Exists(const char* type) {
	return (ini.GetSectionSize(type) >= 0);
}

float ConfigHandler::GetDefault(ConfigType c) {
	switch (c) {
		case ConfigType::iTickRate:
			return 66;
		case ConfigType::fCamSmooth:
			return 3.5f;
		case ConfigType::fIdleOffsetX:
			return 0.0f;
		case ConfigType::fIdleOffsetZ:
			return 0.0f;
		case ConfigType::fCombatOffsetX:
			return 0.0f;
		case ConfigType::fCombatOffsetZ:
			return 0.0f;
		case ConfigType::fMagicOffsetX:
			return 0.0f;
		case ConfigType::fMagicOffsetZ:
			return 0.0f;
		case ConfigType::fBowOffsetX:
			return 0.0f;
		case ConfigType::fBowOffsetZ:
			return 0.0f;
		case ConfigType::fHorseIdleOffsetX:
			return 0.0f;
		case ConfigType::fHorseIdleOffsetZ:
			return 0.0f;
		case ConfigType::fHorseCombatOffsetX:
			return 0.0f;
		case ConfigType::fHorseCombatOffsetZ:
			return 0.0f;
		case ConfigType::fHorseMagicOffsetX:
			return 0.0f;
		case ConfigType::fHorseMagicOffsetZ:
			return 0.0f;
		case ConfigType::fHorseBowOffsetX:
			return 0.0f;
		case ConfigType::fHorseBowOffsetZ:
			return 0.0f;
		case ConfigType::fCamVelocityStrengthX:
			return 5.0f;
		case ConfigType::fSneakIdleOffsetX:
			return 0.0f;
		case ConfigType::fSneakIdleOffsetZ:
			return 0.0f;
		case ConfigType::fSneakCombatOffsetX:
			return 0.0f;
		case ConfigType::fSneakCombatOffsetZ:
			return 0.0f;
		case ConfigType::fSneakMagicOffsetX:
			return 0.0f;
		case ConfigType::fSneakMagicOffsetZ:
			return 0.0f;
		case ConfigType::fSneakBowOffsetX:
			return 0.0f;
		case ConfigType::fSneakBowOffsetZ:
			return 0.0f;
		case ConfigType::fCamVelocityStrengthY:
			return 7.5f;
		case ConfigType::fCamVelocityStrengthZ:
			return 5.0f;
		case ConfigType::iShoulderSwitchKey:
			return 26;
		case ConfigType::iShoulder:
			return 1;
	}
	return 0.0f;
}

void ConfigHandler::UpdateFromConfig(ConfigType configtype, float v) {
	configValues[configtype] = v;
}