#pragma once
#include <thread>
#include <functional>
#include "SKSE/GameReferences.h"

template<typename ret, typename arg, typename arg2>
using fn = std::function<ret(arg, arg2)>;

class Actor;
class AimHelperThread {
	private:
		static void threadFunc(fn<void, Actor*, float> func, Actor* target, float wait) {
			while (isRunning) {
				if (target != nullptr) {
					//Prevent execution on player killmove, dead targets
					if(!(*g_thePlayer)->flags2.killMove && target->formType == kFormType_Character && !target->IsDead(1))
						func(target, wait);
				}
				std::this_thread::sleep_for(std::chrono::microseconds(8333));
			}
		}
		static std::thread _t;
		static bool isRunning;
	public:

		static void RequestThread(fn<void, Actor*, float> func, Actor* target, float wait);
		static void StartThread();
		static void StopThread();
};