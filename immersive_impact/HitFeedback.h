#pragma once
#include <string>
#include <vector>
#include <SKSE/GameMenus.h>
#include <SKSE/GameEvents.h>
#include <SKSE/GameThreads.h>
#include <SKSE/PluginAPI.h>
#include <SKSE/Hooks_UI.h>
#include <random>
#define EVENT TESHitEvent

class BingleHitWaitNextFrame : public UIDelegate {
public:
	static BingleHitWaitNextFrame* Create(Actor* target, Actor* attacker, ActiveEffect* ae, TESHitEvent::Flags flags, float bowDivider);
	virtual void Run();
	virtual void Dispose();

private:
	ActiveEffect* ae;
	Actor* attacker;
	Actor* target;
	TESHitEvent::Flags flags;
	float bowDivider;
};

class HitFeedbackHelper : public IMenu {
protected:
	static HitFeedbackHelper* instance;
	bool invoked = false;
	int frameCounter = 0;
public:
	HitFeedbackHelper();
	virtual ~HitFeedbackHelper();

	static HitFeedbackHelper* GetInstance() {
		return instance;
	}

	static void Register();

	static void ForceProcessCommands();

	void InvokeAddTask(UIDelegate* task);

	virtual void Render();
};

class HitFeedback : public BSTEventSink<EVENT> {
protected:
	static HitFeedback* instance;
	static std::string className;
	static bool feedbackEnabled;
public:
	static std::vector<std::pair<Actor*, ActiveEffect*>> storedActiveEffects;
	static float lastDamage;
	HitFeedback();

	static HitFeedback* GetInstance() {
		return instance;
	}
	virtual ~HitFeedback();

	static void InitHook();

	static void ResetHook();

	static void EnableFeedback(bool b);

	virtual EventResult ReceiveEvent(EVENT* evn, EventDispatcher<EVENT>* src) override;
};