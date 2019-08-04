#include "GameLoadWatcher.h"
#include "SKSE/PapyrusEvents.h"
#include "MenuCloseWatcher.h"
#include "EquipWatcher.h"

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
	MenuCloseWatcher::ResetHook();
	EquipWatcher::ResetHook();
	EquipWatcher::ScanArmorWeight();
	return kEvent_Continue;
}
