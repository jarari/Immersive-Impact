#pragma once
#include "simpleini-4.17\SimpleIni.h"
#include "Papyrus.h"
using namespace BingleImmersiveImpact;

enum ConfigType {
	Speed_Offset,
	Speed_LeftOffset,
	Speed_Pre,
	Speed_Swing1h,
	Speed_Swing2h,
	Speed_SwingDag,
	Speed_SwingFist,
	Speed_Post,
	Speed_CustomL_Swing,
	Speed_CustomR_Swing,
	RestrainMovement,
	AimHelper,
	ActivationRangeMul,
	EnableHitFeedback,
	SpeedAdjustment,
	DeflectChanceMul,
	DeflectChanceMax,
	StaggerResetTime,
	StaggerLimit,
	StaggerDamageMax,
	StaggerAny,
	ChargeDistMax,
	AimCompensationStrength,
	AlwaysChargeIn,
	ChargeVelocity,
	EndOfEnumMarker
};
extern const char* ConfigTypeNames[];
extern float configValues[];
class ConfigHandler {
	static CSimpleIniA ini;
	static ConfigHandler *instance;
	static const char* filepath;
public:
	ConfigHandler();
	static void LoadConfig(UInt32 formId, int weapontype = 0, int slot = 0);
	static void SetConfig(UInt32 formId, ConfigType ctype, float v, bool save = false);
	static void SaveConfig();
	static bool Exists(const char *type);
	static void UpdateFromConfig(ConfigType, float);
	static float GetDefault(ConfigType);
};