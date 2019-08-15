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

class BingleHitWaitNextFrame : public TaskDelegate {
public:
	static BingleHitWaitNextFrame* Create(Actor* target, Actor* attacker, ActiveEffect* ae, TESHitEvent::Flags flags, float bowDivider);
	virtual void Run();
	virtual void Dispose();
	Actor* attacker;
	Actor* target;
private:
	ActiveEffect* ae;
	TESHitEvent::Flags flags;
	float bowDivider;
};

class HitFeedbackHelper : public IMenu {
protected:
	static HitFeedbackHelper* instance;
	BingleHitWaitNextFrame* task = nullptr;
	bool invoked = false;
	int frameCounter = 0;
	Actor* t = nullptr;
	Actor* a = nullptr;
public:
	HitFeedbackHelper();
	virtual ~HitFeedbackHelper();

	static HitFeedbackHelper* GetInstance() {
		return instance;
	}

	void SetInstance(HitFeedbackHelper* _i) {
		instance = _i;
	}

	static void Register();

	void RunTask();

	void SetTask(BingleHitWaitNextFrame* t);

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