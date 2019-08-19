#pragma once

#include "Utilities.h"
#include "GameTypes.h"

template <typename T> class BSTEventSink;

enum EventResult
{
	kEvent_Continue = 0,
	kEvent_Abort
};

// 030
template <typename EventT, typename EventArgT = EventT>
class EventDispatcher
{
protected:
	typedef BSTEventSink<EventT> SinkT;

	SimpleLock			lock;				// 000
	tArray<SinkT*>		eventSinks;			// 008
	tArray<SinkT*>		addBuffer;			// 014 - schedule for add
	tArray<SinkT*>		removeBuffer;		// 020 - schedule for remove
	bool				stateFlag;			// 02C - some internal state changed while sending
	char				pad[3];

	MEMBER_FN_PREFIX(EventDispatcher);
	DEFINE_MEMBER_FN(AddEventSink_Internal, void, 0x006E3E30, SinkT * eventSink);
	DEFINE_MEMBER_FN(RemoveEventSink_Internal, void, 0x008CE0C0, SinkT * eventSink);
	DEFINE_MEMBER_FN(SendEvent_Internal, void, 0x006EBC10, EventArgT * evn);

public:

	EventDispatcher() : stateFlag(false) {}

	void AddEventSink(SinkT * eventSink)		{ CALL_MEMBER_FN(this,AddEventSink_Internal)(eventSink); }
	void RemoveEventSink(SinkT * eventSink)		{ CALL_MEMBER_FN(this,RemoveEventSink_Internal)(eventSink); }
	void SendEvent(EventArgT * evn)				{ CALL_MEMBER_FN(this,SendEvent_Internal)(evn); }
};
STATIC_ASSERT(sizeof(EventDispatcher<void*>) == 0x30);

// 04
template <typename T>
class BSTEventSink
{
public:
	virtual ~BSTEventSink();
	virtual	EventResult	ReceiveEvent(T * evn, EventDispatcher<T> * dispatcher); // pure
//	void	** _vtbl;	// 00
};

// 30
template <typename T>
class BSTEventSource
{
public:
	virtual ~BSTEventSource();

//	void	** _vtbl;	// 00
	UInt32 unk04[11];	// 04
};

STATIC_ASSERT(sizeof(BSTEventSource<void*>) == 0x30);

#define DEFINE_EVENTSINK(sinkName) template <> class BSTEventSink <sinkName> {public:	virtual ~BSTEventSink() {}	virtual	EventResult ReceiveEvent(sinkName * evn, EventDispatcher<sinkName> * dispatcher) = 0;};
// 08
struct MenuOpenCloseEvent
{
	BSFixedString	menuName;	// 00
	bool			opening;	// 04
	char			pad[3];
};

DEFINE_EVENTSINK(MenuOpenCloseEvent);

// Todo
struct MenuModeChangeEvent
{
};

class TESObjectREFR;
class TESForm;
class ActiveEffect;

// This isn't necessarily correct, just there to receive events
struct TESHarvestEvent
{
	struct ItemHarvested
	{
		// Unknown
	};
};

struct LevelIncrease
{
	struct Event
	{
		TESForm		* character;
		UInt32		level;
	};
};

struct SkillIncrease
{
	struct Event
	{
		// Unknown
	};
};
struct WordLearned
{
	struct Event
	{
		// Unknown
	};
};
struct WordUnlocked
{
	struct Event
	{
		// Unknown
	};
};
struct Inventory
{
	struct Event
	{
		// Unknown
	};
};
struct Bounty
{
	struct Event
	{
		// Unknown
	};
};
struct QuestStatus
{
	struct Event
	{
		// Unknown
	};
};
struct ObjectiveState
{
	struct Event
	{
		// Unknown
	};
};
struct Trespass
{
	struct Event
	{
		// Unknown
	};
};
struct FinePaid
{
	struct Event
	{
		// Unknown
	};
};
struct HoursPassed
{
	struct Event
	{
		// Unknown
	};
};
struct DaysPassed
{
	struct Event
	{
		// Unknown
	};
};
struct DaysJailed
{
	struct Event
	{
		// Unknown
	};
};
struct CriticalHitEvent
{
	struct Event
	{
		// Unknown
	};
};
struct DisarmedEvent
{
	struct Event
	{
		// Unknown
	};
};
struct ItemsPickpocketed
{
	struct Event
	{
		// Unknown
	};
};
struct ItemSteal
{
	struct Event
	{
		// Unknown
	};
};
struct ItemCrafted
{
	struct Event
	{
		// Unknown
	};
};
struct LocationDiscovery
{
	struct Event
	{
		// Unknown
	};
};
struct Jailing
{
	struct Event
	{
		// Unknown
	};
};
struct ChestsLooted
{
	struct Event
	{
		// Unknown
	};
};
struct TimesTrained
{
	struct Event
	{
		// Unknown
	};
};
struct TimesBartered
{
	struct Event
	{
		// Unknown
	};
};
struct ContractedDisease
{
	struct Event
	{
		// Unknown
	};
};
struct SpellsLearned
{
	struct Event
	{
		// Unknown
	};
};
struct DragonSoulGained
{
	struct Event
	{
		// Unknown
	};
};
struct SoulGemsUsed
{
	struct Event
	{
		// Unknown
	};
};
struct SoulsTrapped
{
	struct Event
	{
		// Unknown
	};
};
struct PoisonedWeapon
{
	struct Event
	{
		// Unknown
	};
};
struct ShoutAttack
{
	struct Event
	{
		// Unknown
	};
};
struct JailEscape
{
	struct Event
	{
		// Unknown
	};
};
struct GrandTheftHorse
{
	struct Event
	{
		// Unknown
	};
};
struct AssaultCrime
{
	struct Event
	{
		// Unknown
	};
};
struct MurderCrime
{
	struct Event
	{
		// Unknown
	};
};
struct LocksPicked
{
	struct Event
	{
		// Unknown
	};
};
struct LocationCleared
{
	struct Event
	{
		// Unknown
	};
};
struct ShoutMastered
{
	struct Event
	{
		// Unknown
	};
};

struct WeaponAttack {
	struct Event {
		/*
		007426D1: 01B39C5C.SendEvent(01B40324)
		struct `anonymous namespace'::FavoriteWeaponHandler +0000
		+0000 02 struct `anonymous namespace'::FavoriteWeaponHandler | 012B610C
		+0000 01 struct `anonymous namespace'::FavoriteItemHandler<class TESObjectWEAP,struct WeaponAttack::Event> | 012B6008
		-->		+0000 00 class BSTEventSink<struct WeaponAttack::Event> | 012B5F9C
		*/
	};
	static EventDispatcher<Event>* GetEventSource();
};
DEFINE_EVENTSINK(WeaponAttack::Event);

struct BGSFootstepEvent
{
	UInt32	actorHandle;
};


struct BGSEventProcessedEvent {
};

// 8
struct TESActivateEvent {
	TESObjectREFR	* target;		// 00 - TESObjectREFRPtr
	TESObjectREFR	* caster;		// 04 - TESObjectREFRPtr
};

struct TESActiveEffectApplyRemoveEvent {
	TESObjectREFR	* caster;
	TESObjectREFR	* target;
	UInt32			unk08;
	UInt32			unk0C;
	UInt32			unk10;
	UInt32			unk14;
	UInt32			unk18;			// Flags?
	UInt32			unk1C;			// Use effect2 if this is 1
	TESForm			* source;		// Not really sure what this is, probably the extra form
	ActiveEffect	* effect1;
	ActiveEffect	* effect2;
};

// 0C
struct TESActorLocationChangeEvent {
	UInt32 unk00;
	UInt32 unk01;
	UInt32 unk02;
};

struct TESBookReadEvent {
};

struct TESCellAttachDetachEvent {
	// SendEvent: 004CEB5F
	TESObjectREFR	* ref;
	bool			bAttach;
	UInt8			pad[3];
};

struct TESCellReadyToApplyDecalsEvent {
	/*
	004CB1B8: 012E4D80.SendEvent(01310448)
	class `anonymous namespace'::DecalApplier +0000
	+0000 01 class `anonymous namespace'::DecalApplier | 01244C0C
	-->	+0000 00 class BSTEventSink<struct TESCellReadyToApplyDecalsEvent> | 01244288
	*/
};

class TESObjectCELL;
// 04
struct TESCellFullyLoadedEvent {
	// SendEvent: 00437663 <= 4CDD8E
	TESObjectCELL	* cell;
};
DEFINE_EVENTSINK(TESCellFullyLoadedEvent);

class Actor;
// 0C
struct TESCombatEvent {
	Actor	*caster;
	Actor	*target;
	UInt32	state;
};

// 18
struct TESContainerChangedEvent {
	UInt32		from;
	UInt32		to;
	UInt32		item;
	UInt32		count;
	UInt32	toReference;
	UInt32		unk14;
};

struct TESDeathEvent {
};


struct TESDestructionStageChangedEvent {
};

struct TESEnterBleedoutEvent {
};

// 10
struct TESEquipEvent {
	UInt32	unk_00; //Actor
	UInt32	unk_01; //Form ID
	UInt32	unk_02;
	UInt32	unk_03; //Equipping? or unequipping?
};
DEFINE_EVENTSINK(TESEquipEvent);

struct TESFormDeleteEvent {
};

struct TESFurnitureEvent {
};

struct TESGrabReleaseEvent {
};

struct TESHitEvent {
	TESObjectREFR	*target;			// 00
	TESObjectREFR	*caster;			// 04
	UInt32			sourceFormID;		// 08
	UInt32			projectileFormID;	// 0C

	struct Flags {
		bool powerAttack : 1;
		bool sneakAttack : 1;
		bool bash : 1;
		bool blocked : 1;
	} flags;							// 10

	void			*unk14[7];			// 14
};
DEFINE_EVENTSINK(TESHitEvent);

struct TESInitScriptEvent {
	TESObjectREFR	*reference;
	UInt32			unk04;
};


struct TESLoadGameEvent {
};
DEFINE_EVENTSINK(TESLoadGameEvent);

struct TESLockChangedEvent {
};

// 0C
struct TESMagicEffectApplyEvent {
	TESObjectREFR	*target;
	TESObjectREFR	*caster;
	UInt32          unk08;
};

struct TESMagicWardHitEvent {
};

struct TESMoveAttachDetachEvent {
};

struct TESObjectLoadedEvent {
	UInt32	formID;
};

struct TESObjectREFRTranslationEvent {
};

struct TESOpenCloseEvent {
};

struct TESPackageEvent {
};

struct TESPerkEntryRunEvent {
};

struct TESQuestInitEvent {
	UInt32	formId;
};

struct TESQuestStageEvent {
	void			*finishedCallback;
	UInt32			formId;
	UInt32			stage;
};

struct TESQuestStageItemDoneEvent {
};

struct TESQuestStartStopEvent {
};

struct TESResetEvent {
};

struct TESResolveNPCTemplatesEvent {
};

struct TESSceneEvent {
};

struct TESSceneActionEvent {
};

struct TESScenePhaseEvent {
};

struct TESSellEvent {
};

// 08
struct TESSleepStartEvent {
	float	startTime;		// 00
	float	endTime;		// 04
};

struct TESSleepStopEvent {
};

struct TESSpellCastEvent {
};

struct TESPlayerBowShotEvent {
};

struct TESTrackedStatsEvent {
};

struct TESTrapHitEvent {
};

class Actor;
struct TESTopicInfoEvent {
	Actor	*speaker;		// 00 - NiTPointer<Actor>
	void	*unk04;		// 04 - BSTSmartPointer<REFREventCallbacks::IEventCallback>
	UInt32  topicInfoID;	// 08
	bool	flag;			// 0C

	inline bool IsStarting() {
		return !flag;
	}
	inline bool IsStopping() {
		return flag;
	}
};

struct TESTriggerEvent {
};

struct TESTriggerLeaveEvent {
};

struct TESTriggerEnterEvent {
};

struct TESUniqueIDChangeEvent	// see: TESV.00482050
{
	UInt32	unk00;		// always 0?
	UInt32	formID1;
	UInt32	formID2;
	UInt16	unk0C;
	UInt16	unk0E;
};

struct TESWaitStartEvent {
};

struct TESWaitStopEvent {
};

struct TESSwitchRaceCompleteEvent {
	Actor	*actor;
};
DEFINE_EVENTSINK(TESSwitchRaceCompleteEvent);


// 9F0
struct ScriptEventSourceHolder :
	public EventDispatcher<BGSEventProcessedEvent>,				// 000
	public EventDispatcher<TESActivateEvent>,					// 030
	public EventDispatcher<TESActiveEffectApplyRemoveEvent>,		// 060
	public EventDispatcher<TESActorLocationChangeEvent>,			// 090
	public EventDispatcher<TESBookReadEvent>,					// 0C0
	public EventDispatcher<TESCellAttachDetachEvent>,			// 0F0
	public EventDispatcher<TESCellFullyLoadedEvent>,				// 120
	public EventDispatcher<TESCellReadyToApplyDecalsEvent>,		// 150
	public EventDispatcher<TESCombatEvent>,						// 180
	public EventDispatcher<TESContainerChangedEvent>,			// 1B0
	public EventDispatcher<TESDeathEvent>,						// 1E0
	public EventDispatcher<TESDestructionStageChangedEvent>,		// 210
	public EventDispatcher<TESEnterBleedoutEvent>,				// 240
	public EventDispatcher<TESEquipEvent>,						// 270
	public EventDispatcher<TESFormDeleteEvent>,					// 2A0
	public EventDispatcher<TESFurnitureEvent>,					// 2D0
	public EventDispatcher<TESGrabReleaseEvent>,					// 300
	public EventDispatcher<TESHitEvent>,							// 330
	public EventDispatcher<TESInitScriptEvent>,					// 360
	public EventDispatcher<TESLoadGameEvent>,					// 390
	public EventDispatcher<TESLockChangedEvent>,					// 3C0
	public EventDispatcher<TESMagicEffectApplyEvent>,			// 3F0
	public EventDispatcher<TESMagicWardHitEvent>,				// 420
	public EventDispatcher<TESMoveAttachDetachEvent>,			// 450
	public EventDispatcher<TESObjectLoadedEvent>,				// 480
	public EventDispatcher<TESObjectREFRTranslationEvent>,		// 4B0
	public EventDispatcher<TESOpenCloseEvent>,					// 4E0
	public EventDispatcher<TESPackageEvent>,						// 510
	public EventDispatcher<TESPerkEntryRunEvent>,				// 540
	public EventDispatcher<TESQuestInitEvent>,					// 570
	public EventDispatcher<TESQuestStageEvent>,					// 5A0
	public EventDispatcher<TESQuestStageItemDoneEvent>,			// 5D0
	public EventDispatcher<TESQuestStartStopEvent>,				// 600
	public EventDispatcher<TESResetEvent>,						// 630
	public EventDispatcher<TESResolveNPCTemplatesEvent>,			// 660
	public EventDispatcher<TESSceneEvent>,						// 690
	public EventDispatcher<TESSceneActionEvent>,					// 6C0
	public EventDispatcher<TESScenePhaseEvent>,					// 6F0
	public EventDispatcher<TESSellEvent>,						// 720
	public EventDispatcher<TESSleepStartEvent>,					// 750
	public EventDispatcher<TESSleepStopEvent>,					// 780
	public EventDispatcher<TESSpellCastEvent>,					// 7B0
	public EventDispatcher<TESPlayerBowShotEvent>,				// 7E0
	public EventDispatcher<TESTopicInfoEvent>,					// 810
	public EventDispatcher<TESTrackedStatsEvent>,				// 840
	public EventDispatcher<TESTrapHitEvent>,						// 870
	public EventDispatcher<TESTriggerEvent>,						// 8A0
	public EventDispatcher<TESTriggerEnterEvent>,				// 8D0
	public EventDispatcher<TESTriggerLeaveEvent>,				// 900
	public EventDispatcher<TESUniqueIDChangeEvent>,				// 930
	public EventDispatcher<TESWaitStartEvent>,					// 960
	public EventDispatcher<TESWaitStopEvent>,					// 990
	public EventDispatcher<TESSwitchRaceCompleteEvent>			// 9C0
{
	static ScriptEventSourceHolder* GetInstance(void) {			// 00438740
		ScriptEventSourceHolder *pThis = (ScriptEventSourceHolder*)0x012E4C30;
		//bool &bInit = *(bool*)(pThis + 1);
		//if (!bInit) {
		//	pThis->ctor();
		//	bInit = true;
		//}
		return pThis;
	}

	template <class EventT>
	EventDispatcher<EventT>* GetEventSource() {
		return static_cast<EventDispatcher<EventT>*>(this);
	}

	MEMBER_FN_PREFIX(ScriptEventSourceHolder);
	DEFINE_MEMBER_FN(SendActivateEvent, void, 0x004E0450, BSTSmartPointer<TESObjectREFR> &target, BSTSmartPointer<TESObjectREFR> &caster);
	//DEFINE_MEMBER_FN(SendTopicInfoEvent,					void,	0x0057DDD0,	FormID topicInfoID, ActorPtr &speaker, UInt32 flag, BSTSmartPointer<REFREventCallbacks::IEventCallback> &arg4);
	//DEFINE_MEMBER_FN(SendTESObjectREFRTranslationEvent,	void,	0x004CB220,	UInt32 arg1, UInt32 arg2);


};
STATIC_ASSERT(sizeof(ScriptEventSourceHolder) == 0x9F0);


extern EventDispatcher<BGSEventProcessedEvent>&				g_eventProcessedEventSource;
extern EventDispatcher<TESActivateEvent>&					g_activateEventSource;
extern EventDispatcher<TESActiveEffectApplyRemoveEvent>&		g_activeEffectApplyRemoveEventSource;
extern EventDispatcher<TESActorLocationChangeEvent>&			g_actorLocationChangeEventSource;
extern EventDispatcher<TESBookReadEvent>&					g_bookReadEventSource;
extern EventDispatcher<TESCellAttachDetachEvent>&			g_cellAttachDetachEventSource;
extern EventDispatcher<TESCellFullyLoadedEvent>&				g_cellFullyLoadedEventSource;
extern EventDispatcher<TESCellReadyToApplyDecalsEvent>&		g_cellReadyToApplyDecalsEventSource;
extern EventDispatcher<TESCombatEvent>&						g_combatEventSource;
extern EventDispatcher<TESContainerChangedEvent>&			g_containerChangedEventSource;
extern EventDispatcher<TESDeathEvent>&						g_deathEventSource;
extern EventDispatcher<TESDestructionStageChangedEvent>&		g_destructionStageChangedEventSource;
extern EventDispatcher<TESEnterBleedoutEvent>&				g_enterBleedoutEventSource;
extern EventDispatcher<TESEquipEvent>&						g_equipEventSource;
extern EventDispatcher<TESFormDeleteEvent>&					g_formDeleteEventSource;
extern EventDispatcher<TESFurnitureEvent>&					g_furnitureEventSource;
extern EventDispatcher<TESGrabReleaseEvent>&					g_grabReleaseEventSource;
extern EventDispatcher<TESHitEvent>&							g_hitEventSource;
extern EventDispatcher<TESInitScriptEvent>&					g_initScriptEventSource;
extern EventDispatcher<TESLoadGameEvent>&					g_loadGameEventSource;
extern EventDispatcher<TESLockChangedEvent>&					g_lockChangedEventSource;
extern EventDispatcher<TESMagicEffectApplyEvent>&			g_magicEffectApplyEventSource;
extern EventDispatcher<TESMagicWardHitEvent>&				g_magicWardHitEventSource;
extern EventDispatcher<TESMoveAttachDetachEvent>&			g_moveAttachDetachEventSource;
extern EventDispatcher<TESObjectLoadedEvent>&				g_objectLoadedEventSource;
extern EventDispatcher<TESObjectREFRTranslationEvent>&		g_objectREFRTranslationEventSource;
extern EventDispatcher<TESOpenCloseEvent>&					g_openCloseEventSource;
extern EventDispatcher<TESPackageEvent>&						g_packageEventSource;
extern EventDispatcher<TESPerkEntryRunEvent>&				g_perkEntryRunEventSource;
extern EventDispatcher<TESQuestInitEvent>&					g_questInitEventSource;
extern EventDispatcher<TESQuestStageEvent>&					g_questStageEventSource;
extern EventDispatcher<TESQuestStageItemDoneEvent>&			g_questStageItemDoneEventSource;
extern EventDispatcher<TESQuestStartStopEvent>&				g_questStartStopEventSource;
extern EventDispatcher<TESResetEvent>&						g_resetEventSource;
extern EventDispatcher<TESResolveNPCTemplatesEvent>&			g_resolveNPCTemplatesEventSource;
extern EventDispatcher<TESSceneEvent>&						g_sceneEventSource;
extern EventDispatcher<TESSceneActionEvent>&					g_sceneActionEventSource;
extern EventDispatcher<TESScenePhaseEvent>&					g_scenePhaseEventSource;
extern EventDispatcher<TESSellEvent>&						g_sellEventSource;
extern EventDispatcher<TESSleepStartEvent>&					g_sleepStartEventSource;
extern EventDispatcher<TESSleepStopEvent>&					g_sleepStopEventSource;
extern EventDispatcher<TESSpellCastEvent>&					g_spellCastEventSource;
extern EventDispatcher<TESPlayerBowShotEvent>&				g_playerBowShotEventSource;
extern EventDispatcher<TESTopicInfoEvent>&					g_topicInfoEventSource;
extern EventDispatcher<TESTrackedStatsEvent>&				g_trackedStatsEventSource;
extern EventDispatcher<TESTrapHitEvent>&						g_trapHitEventSource;
extern EventDispatcher<TESTriggerEvent>&						g_triggerEventSource;
extern EventDispatcher<TESTriggerEnterEvent>&				g_triggerEnterEventSource;
extern EventDispatcher<TESTriggerLeaveEvent>&				g_triggerLeaveEventSource;
extern EventDispatcher<TESUniqueIDChangeEvent>&				g_uniqueIDChangeEventSource;
extern EventDispatcher<TESWaitStartEvent>&					g_waitStartEventSource;
extern EventDispatcher<TESWaitStopEvent>&					g_waitStopEventSource;
extern EventDispatcher<TESSwitchRaceCompleteEvent>&			g_switchRaceCompleteEventSource;