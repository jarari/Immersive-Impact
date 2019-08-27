#include "AimHelperThread.h"
#include "BingleEventInvoker.h"

bool AimHelperThread::isRunning = false;
std::thread AimHelperThread::_t;

void AimHelperThread::threadFunc(fn<void, Actor*, float> func, Actor* target, float size) {
	bool brake = false;
	while (isRunning && !brake) {
		if (target != nullptr) {
			//Prevent execution on player killmove, dead targets
			if (!(*g_thePlayer)->flags2.killMove && target->formType == kFormType_Character && !target->IsDead(1)) {
				func(target, size);
			}
			else {
				brake = true;
				BingleEventInvoker::TranslateToTarget(*g_thePlayer);
				BingleEventInvoker::StopTranslation();
			}
		}
		std::this_thread::sleep_for(std::chrono::microseconds(8333));
	}
}

void AimHelperThread::RequestThread(fn<void, Actor*, float> func, Actor* target, float size) {
	if (isRunning) {
		StopThread();
	}
	_t = std::thread(&threadFunc, func, target, size);
	StartThread();
}

void AimHelperThread::StartThread() {
	if (isRunning)
		return;
	isRunning = true;
}

void AimHelperThread::StopThread() {
	if (isRunning) {
		isRunning = false;
		if (_t.joinable()) {
			_t.join();
			_t.std::thread::~thread();
		}
	}
}