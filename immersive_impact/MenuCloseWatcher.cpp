#include "MenuCloseWatcher.h"
#include "SKSE/PapyrusActor.h"
#include "BingleEventInvoker.h"
#include "SKSE/GameMenus.h"
#include "EquipWatcher.h"
#include "HitFeedback.h"
#include <SKSE\SafeWrite.h>

MenuCloseWatcher *MenuCloseWatcher::instance = nullptr;
bool MenuCloseWatcher::actionRequested = false;
Actor *MenuCloseWatcher::actionTarget = nullptr;

bool helperWasOpen = false;
BSFixedString closeCalledBy;

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
	helperWasOpen = false;
}

EventResult MenuCloseWatcher::ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent>* src) {
	MenuManager* mm = MenuManager::GetSingleton();
	UIManager* ui = UIManager::GetSingleton();
	UIStringHolder* uistr = UIStringHolder::GetSingleton();
	if (uistr && evn->menuName == uistr->mainMenu && !evn->opening) {
		HitFeedback::ResetHook();
		g_mainScriptRegs.Clear();
		MenuCloseWatcher::ResetHook();
		if (mm && !mm->IsMenuOpen(&BSFixedString("HitFeedbackHelper")) && ui)
			CALL_MEMBER_FN(ui, AddMessage)(&StringCache::Ref("HitFeedbackHelper"), UIMessage::kMessage_Open, nullptr);
		if (EquipWatcher::isInitialized) {
			EquipWatcher::ResetHook();
		}
	}
	else if (uistr && evn->menuName == uistr->loadingMenu && !evn->opening) {
		if (mm && !mm->IsMenuOpen(&BSFixedString("HitFeedbackHelper")) && ui)
			CALL_MEMBER_FN(ui, AddMessage)(&StringCache::Ref("HitFeedbackHelper"), UIMessage::kMessage_Open, nullptr);
	}
	return kEvent_Continue;
}
