#pragma once

namespace BingleImmersiveImpact {
	enum ConfigType{
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
		ChargeMul,
		HitFeedback,
		SpeedAdjustment,
		DeflectChanceMul,
		DeflectChanceMax,
		StaggerResetTime,
		StaggerLimit,
		StaggerDamageMax,
		StaggerAny,
		EndOfEnumMarker
	};
	static const char* ConfigTypeNames[ConfigType::EndOfEnumMarker] = {
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
		"ChargeMul",
		"HitFeedback",
		"SpeedAdjustment",
		"DeflectChanceMul",
		"DeflectChanceMax",
		"StaggerResetTime",
		"StaggerLimit",
		"StaggerDamageMax",
		"StaggerAny"
	};
	float speedValues[];
	void UpdateFromConfig(ConfigType, float);
	float GetDefault(ConfigType);
	void SetCustomized(int slot, bool b);
	void EnableSpeedAdjustment(bool b);
}

//Register papyrus functions
class VMClassRegistry;
namespace Papyrus {
	bool RegisterFuncs(VMClassRegistry* registry);
};