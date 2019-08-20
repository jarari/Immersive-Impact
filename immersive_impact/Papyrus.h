#pragma once

namespace BingleImmersiveImpact {
	void SetCustomized(int slot, bool b);
	void EnableSpeedAdjustment(bool b);
}

//Register papyrus functions
class VMClassRegistry;
namespace Papyrus {
	bool RegisterFuncs(VMClassRegistry* registry);
};