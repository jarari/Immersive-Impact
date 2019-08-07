#include "AimHelperThread.h"

bool AimHelperThread::isRunning = false;
std::thread AimHelperThread::_t;

void AimHelperThread::RequestThread(fn<void, Actor*, float> func, Actor* target, float wait) {
	if (isRunning) {
		StopThread();
	}
	_t = std::thread(&threadFunc, func, target, wait);
	StartThread();
}

void AimHelperThread::StartThread() {
	if (isRunning)
		return;
	isRunning = true;
}

void AimHelperThread::StopThread() {
	if (!isRunning)
		return;
	isRunning = false;
	_t.join();
	_t.std::thread::~thread();
}