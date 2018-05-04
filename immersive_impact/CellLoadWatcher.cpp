#include "CellLoadWatcher.h"
#include "SKSE/PapyrusEvents.h"
#include "MenuCloseWatcher.h"
#include "EquipWatcher.h"

CellLoadWatcher *CellLoadWatcher::instance = nullptr;

CellLoadWatcher::~CellLoadWatcher() {
}

void CellLoadWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new CellLoadWatcher();
	g_cellFullyLoadedEventSource.AddEventSink((BSTEventSink<TESCellFullyLoadedEvent>*)instance);
	_MESSAGE("Cell load hook added to the sink.");
}

EventResult CellLoadWatcher::ReceiveEvent(TESCellFullyLoadedEvent * evn, EventDispatcher<TESCellFullyLoadedEvent>* src) {
	_MESSAGE("New cell loaded.");
	MenuCloseWatcher::ResetHook();
	EquipWatcher::ResetHook();
	return kEvent_Continue;
}
