#pragma once
#include "SKSE/GameEvents.h"

class GameLoadWatcher : public BSTEventSink<TESLoadGameEvent> {
protected:
	static GameLoadWatcher*instance;
public:
	virtual ~GameLoadWatcher();

	static void InitHook();

	virtual EventResult ReceiveEvent(TESLoadGameEvent* evn, EventDispatcher<TESLoadGameEvent>* src) override;
};
