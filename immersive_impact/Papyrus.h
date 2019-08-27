#pragma once

namespace BingleImmersiveImpact {
	void SetCustomized(int slot, bool b);
	void EnableSpeedAdjustment(bool b);
	bool isSpeedAdjustmentEnabled();
}

//Register papyrus functions
class VMClassRegistry;
namespace Papyrus {
	bool RegisterFuncs(VMClassRegistry* registry);
};