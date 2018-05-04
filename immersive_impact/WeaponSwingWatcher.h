#pragma once
#include "SKSE/GameEvents.h"

template <typename FormT, typename DataT>
class CustomHandlerFunctor {
public:
	virtual ~CustomHandlerFunctor();
	virtual	UInt8	Process(FormT * form, DataT unk04); // pure

	BSIntrusiveRefCounted refCount;
};

template<>
class CustomHandlerFunctor<Actor, UInt32> {
public:
	virtual ~CustomHandlerFunctor();
	virtual	UInt8	Process(Actor * form, UInt32 unk04); // pure

	template<class T, UInt32 Type>
	UInt8	ProcessAction(Actor * form, UInt32 unk04);
};

class AttackStopHandler : public CustomHandlerFunctor<Actor, UInt32> {
public:
	virtual ~AttackStopHandler();
	virtual	UInt8	Process(Actor * form, UInt32 unk04);

	MEMBER_FN_PREFIX(AttackStopHandler);
	DEFINE_MEMBER_FN(Process_Origin, UInt8, 0x00780790, Actor * form, UInt32 unk04);
};

class AttackWinStartHandler : public CustomHandlerFunctor<Actor, UInt32> {
public:
	virtual ~AttackWinStartHandler();
	virtual	UInt8	Process(Actor * form, UInt32 unk04);

	MEMBER_FN_PREFIX(AttackWinStartHandler);
	DEFINE_MEMBER_FN(Process_Origin, UInt8, 0x00780790, Actor * form, UInt32 unk04);
};

class Actor;
class WeaponSwingWatcher {
protected:
	static WeaponSwingWatcher *instance;
public:
	WeaponSwingWatcher() {
		if (instance)
			delete(instance);
		instance = this;
		_MESSAGE("WeaponSwingWatcher instance created.");
	}

	static WeaponSwingWatcher *GetInstance() {
		return instance;
	}
	virtual ~WeaponSwingWatcher();

	static void InitHook();

	static void ResetHook();
};
