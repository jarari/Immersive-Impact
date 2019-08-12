#include "MenuCloseWatcher.h"
#include "SKSE/PapyrusActor.h"
#include "SKSE/PapyrusEvents.h"
#include "SKSE/GameMenus.h"

MenuCloseWatcher *MenuCloseWatcher::instance = nullptr;
bool MenuCloseWatcher::actionRequested = false;
Actor *MenuCloseWatcher::actionTarget = nullptr;

MenuCloseWatcher::~MenuCloseWatcher() {
}

void MenuCloseWatcher::RequestAction(Actor *a) {
	actionRequested = true;
	actionTarget = a;
}

void MenuCloseWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new MenuCloseWatcher();
	MenuManager * mm = MenuManager::GetSingleton();
	if (mm) {
		mm->MenuOpenCloseEventDispatcher()->AddEventSink((BSTEventSink<MenuOpenCloseEvent>*)instance);
		_MESSAGE("MenuCloseWatcher added to the sink.");
	}
}

void MenuCloseWatcher::ResetHook() {
	actionRequested = false;
	actionTarget = nullptr;
}

EventResult MenuCloseWatcher::ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent>* src) {
	if (actionRequested && !evn->opening) {
		actionRequested = false;
		_MESSAGE("Action requested, and the user is closing the menu...");
		if (!actionTarget->GetNiNode())
			return kEvent_Continue;
		if (!actionTarget->GetEquippedObject(false) && !actionTarget->GetEquippedObject(true)
			&& !actionTarget->equippingMagicItems[0] && !actionTarget->equippingMagicItems[1]) {
			_MESSAGE("...and he's unarmed. Force equipping the hand weapon.");
			BingleEventInvoker::EquipFist(actionTarget);
		}
	}
	return kEvent_Continue;
}
