#include "CameraController.h"
#include "MenuCloseWatcher.h"
#include <SKSE\GameMenus.h>

MenuCloseWatcher* MenuCloseWatcher::instance = nullptr;

MenuCloseWatcher::~MenuCloseWatcher() {
}

void MenuCloseWatcher::InitHook() {
	if (instance)
		delete(instance);
	instance = new MenuCloseWatcher();
	MenuManager* mm = MenuManager::GetSingleton();
	if (mm) {
		mm->MenuOpenCloseEventDispatcher()->AddEventSink((BSTEventSink<MenuOpenCloseEvent>*)instance);
		_MESSAGE("MenuCloseWatcher added to the sink.");
	}
}

EventResult MenuCloseWatcher::ReceiveEvent(MenuOpenCloseEvent* evn, EventDispatcher<MenuOpenCloseEvent>* src) {
	MenuManager* mm = MenuManager::GetSingleton();
	UIManager* ui = UIManager::GetSingleton();
	UIStringHolder* uistr = UIStringHolder::GetSingleton();
	if (uistr && evn->menuName == uistr->loadingMenu && evn->opening) {
		CameraController::hookActive = false;
	}
	else if (uistr && evn->menuName == uistr->loadingMenu && !evn->opening) {
		CameraController::hookActive = true;
	}
	else if (uistr && evn->menuName == uistr->mainMenu && evn->opening) {
		CameraController::hookActive = false;
	}
	return kEvent_Continue;
}