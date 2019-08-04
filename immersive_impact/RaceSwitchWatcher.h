#pragma once
#include "SKSE/GameEvents.h"
#include <string>

class RaceSwitchWatcher : public BSTEventSink<TESSwitchRaceCompleteEvent> {
protected:
	static RaceSwitchWatcher *instance;
	static std::string className;
public:
	RaceSwitchWatcher() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE((className + std::string(" instance created.")).c_str());
	}

	RaceSwitchWatcher *GetInstance() {
		return instance;
	}
	virtual ~RaceSwitchWatcher();

	static void InitHook();

	static void ResetHook();

	virtual EventResult ReceiveEvent(TESSwitchRaceCompleteEvent *evn, EventDispatcher<TESSwitchRaceCompleteEvent>* src) override;
};