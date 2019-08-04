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
		"AimHelper"
	};
	float speedValues[];
	void UpdateFromConfig(ConfigType, float);
	float GetDefault(ConfigType);
	void SetCustomized(int slot, bool b);
}

//Register papyrus functions
class VMClassRegistry;
namespace Papyrus {
	bool RegisterFuncs(VMClassRegistry* registry);
};