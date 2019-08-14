#include "RaceSwitchWatcher.h"
#include "EquipWatcher.h"

RaceSwitchWatcher *RaceSwitchWatcher::instance = nullptr;
std::string RaceSwitchWatcher::className = "RaceSwitchWatcher";

void RaceSwitchWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new RaceSwitchWatcher();
	g_switchRaceCompleteEventSource.AddEventSink((BSTEventSink<TESSwitchRaceCompleteEvent>*)instance);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

EventResult RaceSwitchWatcher::ReceiveEvent(TESSwitchRaceCompleteEvent *evn, EventDispatcher<TESSwitchRaceCompleteEvent>* src) {
	if (evn->actor == (Actor*)(*g_thePlayer)) {
		EquipWatcher::ResetHook();
	}
	return kEvent_Continue;
}


RaceSwitchWatcher::~RaceSwitchWatcher() {
}
