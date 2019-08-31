#pragma once
#include "simpleini-4.17\SimpleIni.h"
#include "Papyrus.h"

enum ConfigType {
	iTickRate,
	fCamSmooth,
	fIdleOffsetX,
	fIdleOffsetZ,
	fCombatOffsetX,
	fCombatOffsetZ,
	fMagicOffsetX,
	fMagicOffsetZ,
	fBowOffsetX,
	fBowOffsetZ,
	fHorseIdleOffsetX,
	fHorseIdleOffsetZ,
	fHorseCombatOffsetX,
	fHorseCombatOffsetZ,
	fHorseMagicOffsetX,
	fHorseMagicOffsetZ,
	fHorseBowOffsetX,
	fHorseBowOffsetZ,
	fCamVelocityStrengthX,
	fSneakIdleOffsetX,
	fSneakIdleOffsetZ,
	fSneakCombatOffsetX,
	fSneakCombatOffsetZ,
	fSneakMagicOffsetX,
	fSneakMagicOffsetZ,
	fSneakBowOffsetX,
	fSneakBowOffsetZ,
	fCamVelocityStrengthY,
	fCamVelocityStrengthZ,
	iShoulderSwitchKey,
	iShoulder,
	fVelocityUpdateCycle,
	EndOfEnumMarker
};
extern const char* ConfigTypeNames[];
extern float configValues[];
class ConfigHandler {
	static CSimpleIniA ini;
	static ConfigHandler* instance;
	static const char* filepath;
public:
	static void InitHandler();
	static void LoadConfig();
	static void SetConfig(ConfigType ctype, float v, bool save = false);
	static void SaveConfig();
	static bool Exists(const char* type);
	static void UpdateFromConfig(ConfigType, float);
	static float GetDefault(ConfigType);
};