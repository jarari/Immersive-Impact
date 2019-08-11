#pragma once
#include <thread>
#include <functional>
#include "SKSE/GameReferences.h"

template<typename ret, typename arg, typename arg2>
using fn = std::function<ret(arg, arg2)>;

class Actor;
class AimHelperThread {
	private:
		static void threadFunc(fn<void, Actor*, float> func, Actor* target, float size);
		static std::thread _t;
		static bool isRunning;
	public:

		static void RequestThread(fn<void, Actor*, float> func, Actor* target, float wait);
		static void StartThread();
		static void StopThread();
};