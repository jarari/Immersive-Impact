#pragma once

//THIS IS NOT A TEMPLATE CLASS/PARENT CLASS FOR WATCHERS
//I'm just a fucking lazy bitch so I decided to make this one for copy pasta.
//Copy & Paste this template for additional watchers.

#include <string>
#include "SKSE/GameEvents.h"
#define EVENT 
#define NAME 

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

	static void InitHook();

	static void ResetHook();

	virtual EventResult ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) override;
};

NAME* NAME::instance = nullptr;
std::string NAME::className = "";

void NAME::InitHook() {
	if (instance)
		delete(instance);
	instance = new NAME();
	g_cellFullyLoadedEventSource.AddEventSink((BSTEventSink<EVENT>*)instance);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

EventResult NAME::ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) {
	return kEvent_Continue;
}