#pragma once
#include <string>
#include "SKSE/PapyrusEvents.h"
#define EVENT SKSENiNodeUpdateEvent
#define NAME NiNodeUpdateWatcher

class NAME : public BSTEventSink<EVENT> {
protected:
	static NAME* instance;
	static std::string className;
public:
	NAME() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE((className + std::string(" instance created.")).c_str());
	}

	static NAME* GetInstance() {
		return instance;
	}
	virtual ~NAME();

	static void InitHook(const SKSEMessagingInterface* skse);

	virtual EventResult ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) override;
};