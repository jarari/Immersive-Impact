#pragma once
#include <string.h>

//THIS IS NOT A TEMPLATE CLASS/PARENT CLASS FOR WATCHERS
//I'm just a fucking lazy bitch so I decided to make this one for copy pasta.
//Copy & Paste this template for additional watchers.

#include "SKSE/GameEvents.h"

class WatcherTemplate : public BSTEventSink<EVENT> {
protected:
	static WatcherTemplate *instance;
	static std::string className;
public:
	WatcherTemplate() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE((className + std::string(" instance created.")).c_str());
	}

	WatcherTemplate *GetInstance() {
		return instance;
	}
	virtual ~WatcherTemplate();

	static void InitHook();

	static void ResetHook();

	virtual EventResult ReceiveEvent(EVENT *evn, EventDispatcher<EVENT>* src) override;
};

WatcherTemplate *WatcherTemplate::instance = nullptr;
std::string WatcherTemplate::className = "WatcherTemplate";

void WatcherTemplate::InitHook() {
	if (instance)
		delete(instance);
	instance = new WatcherTemplate();
	g_questStageEventSource.AddEventSink((BSTEventSink<>*)instance);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

EventResult WatcherTemplate::ReceiveEvent(*evn, EventDispatcher<>* src) {
	return kEvent_Continue;
}
