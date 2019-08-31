#include "NiNodeUpdateWatcher.h"
#include "CameraController.h"
#include <SKSE/PluginManager.h>

NAME* NAME::instance = nullptr;
std::string NAME::className = "NiNodeUpdateWatcher";

void NAME::InitHook(const SKSEMessagingInterface* skse) {
	if (instance)
		delete(instance);
	instance = new NAME();
	EventDispatcher<SKSENiNodeUpdateEvent>* dispatcher = (EventDispatcher<SKSENiNodeUpdateEvent>*)skse->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_NiNodeUpdateEvent);
	_MESSAGE("Dispatcher 0x%08x", (UInt32)dispatcher);
	dispatcher->AddEventSink((BSTEventSink<EVENT>*)instance);
	_MESSAGE((className + std::string(" added to the sink.")).c_str());
}

NAME::~NAME() {
}

EventResult NAME::ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* dispatcher) {
	if (evn->reference == *g_thePlayer) {
		_MESSAGE("NiNode updated.");
		PlayerCamera* pCam = PlayerCamera::GetSingleton();
		if (pCam) {
			CameraController::HookCharacterMoveFinishEvent(pCam);
		}
	}
	return kEvent_Continue;
}