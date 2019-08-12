#pragma once
#include <string>
#include <vector>
#include "SKSE/GameEvents.h"
#define EVENT TESHitEvent

class HitFeedback : public BSTEventSink<EVENT> {
protected:
	static HitFeedback* instance;
	static std::string className;
	static bool feedbackEnabled;
public:
	static std::vector<std::pair<Actor*, ActiveEffect*>> storedActiveEffects;
	HitFeedback() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE((className + std::string(" instance created.")).c_str());
	}

	HitFeedback* GetInstance() {
		return instance;
	}
	virtual ~HitFeedback();

	static void InitHook();

	static void ResetHook();

	static void EnableFeedback(bool b);

	virtual EventResult ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) override;
};