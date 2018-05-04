#pragma once
#include "SKSE/GameEvents.h"

class Actor;
class MenuCloseWatcher : public BSTEventSink<MenuOpenCloseEvent> {
protected:
	static MenuCloseWatcher *instance;
	static bool actionRequested;
	static Actor *actionTarget;
public:
	MenuCloseWatcher() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE("MenuCloseWatcher instance created.");
	}

	static MenuCloseWatcher *GetInstance() {
		return instance;
	}
	virtual ~MenuCloseWatcher();

	static void RequestAction(Actor *a);

	static void InitHook();

	static void ResetHook();

	virtual EventResult ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent>* src) override;
};
