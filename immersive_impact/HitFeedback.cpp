#include "HitFeedback.h"

HitFeedback* HitFeedback::instance = nullptr;
std::string HitFeedback::className = "HitFeedback";

HitFeedback::~HitFeedback() {
}

void HitFeedback::InitHook() {
	if (instance)
		delete(instance);
	instance = new HitFeedback();
	g_hitEventSource.AddEventSink((BSTEventSink<EVENT>*)instance);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

EventResult HitFeedback::ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) {
	return kEvent_Continue;
}
