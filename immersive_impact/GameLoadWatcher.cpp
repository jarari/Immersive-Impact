#include "GameLoadWatcher.h"
#include "BingleEventInvoker.h"
#include "MenuCloseWatcher.h"
#include "EquipWatcher.h"
#include "HitFeedback.h"

GameLoadWatcher* GameLoadWatcher::instance = nullptr;

GameLoadWatcher::~GameLoadWatcher() {
}

void GameLoadWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new GameLoadWatcher();
	g_loadGameEventSource.AddEventSink((BSTEventSink<TESLoadGameEvent>*)instance);
	_MESSAGE("Game load hook added to the sink.");
}

EventResult GameLoadWatcher::ReceiveEvent(TESLoadGameEvent* evn, EventDispatcher<TESLoadGameEvent>* src) {
	_MESSAGE("New game loaded.");
	g_mainScriptRegs.Clear();
	MenuCloseWatcher::ResetHook();
	EquipWatcher::ResetHook();
	HitFeedback::ResetHook();
	return kEvent_Continue;
}
