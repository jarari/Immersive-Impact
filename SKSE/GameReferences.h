#pragma once

#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/GameEvents.h"
#include "skse/NiObjects.h"
#include "GameBSExtraData.h"

class BSAnimationGraphEvent;
class NiNode;
class TESObjectREFR;
class BSFaceGenNiNode;
class BSFaceGenAnimationData;

// TESObjectREFR and child classes

// 08
class BSHandleRefObject : public NiRefObject
{
public:
	enum
	{
		kMask_RefCount = 0x3FF
	};

	UInt32 GetRefCount() const
	{
		return m_uiRefCount & kMask_RefCount;
	}

	void DecRefHandle()
	{
		if((InterlockedDecrement(&m_uiRefCount) & kMask_RefCount) == 0)
			DeleteThis();
	}
};

// The refHandle is stored in BSHandleRefObject.m_uiRefCount
// Bits:
//	01 - 10	ref count
//	11		isHandleSet
//	12 - ??	handle (seems to be divided in two parts, 0xFFFFF and 0x3F00000)

// Adds entry to lookup table if not yet there
typedef void (* _CreateRefHandleByREFR)(UInt32 * refHandleOut, TESObjectREFR * refr);
extern const _CreateRefHandleByREFR CreateRefHandleByREFR;

// Note: May set refHandle to 0
typedef bool (* _LookupREFRByHandle)(UInt32 * refHandle, TESObjectREFR ** refrOut);
extern const _LookupREFRByHandle LookupREFRByHandle;

typedef bool (* _LookupREFRObjectByHandle)(UInt32 * refHandle, BSHandleRefObject ** refrOut);
extern const _LookupREFRObjectByHandle LookupREFRObjectByHandle;

extern const UInt32 * g_invalidRefHandle;

typedef void (*_MoveRefrToPosition)(TESObjectREFR* source, UInt32* pTargetHandle, void* parentCell, void* worldSpace, NiPoint3* postion, NiPoint3* rotation);
extern const _MoveRefrToPosition MoveRefrToPosition;

// 4
class IAnimationGraphManagerHolder
{
public:
	virtual ~IAnimationGraphManagerHolder();

	virtual bool	SendAnimationEvent(const BSFixedString &eventName);								// 01 0064F7D0
	virtual UInt32 Unk_02();					// 02 00F51EE8
	virtual bool	IAnimationGraphManagerHolder_Unk_03(UInt32 arg);							// 03 00F51EE8 (pure)
	virtual bool	IAnimationGraphManagerHolder_Unk_04(UInt32 arg);								// 04 004091A0 { return false; }
	virtual bool	IAnimationGraphManagerHolder_Unk_05(UInt32 arg);							// 05 00F51EE8 (pure)
	virtual bool	IAnimationGraphManagerHolder_Unk_06(void);										// 06 009B86F0 { return true; } Actor={ return false; }
	virtual bool	IAnimationGraphManagerHolder_Unk_07(UInt32 arg);								// 07 00C8CCA0 { return true; }
	virtual bool	IAnimationGraphManagerHolder_Unk_08(UInt32 arg);								// 08 00C8CCA0 { return true; }
	virtual void	IAnimationGraphManagerHolder_Unk_09(void);										// 09 006C50E0 { return; }
	virtual bool	IAnimationGraphManagerHolder_Unk_0A(UInt32 arg);								// 0A 004091A0 { return false; }
	virtual void	IAnimationGraphManagerHolder_Unk_0B(UInt32 arg);								// 0B 00588F30 { return; }
	virtual void	IAnimationGraphManagerHolder_Unk_0C(void);										// 0C 006C50E0 { return; }
	virtual void	IAnimationGraphManagerHolder_Unk_0D(UInt32 arg1, UInt32 arg2);					// 0D 004D43E0 { return; }
	virtual bool	IAnimationGraphManagerHolder_Unk_0E(void);										// 0E 009B86F0 { return true; }
	virtual UInt32	IAnimationGraphManagerHolder_Unk_0F(void);										// 0F 005EADD0 { return nullptr; }
	virtual bool	GetAnimationVariableFloat(const BSFixedString &variableName, float &out);		// 10 00650C40
	virtual bool	GetAnimationVariableInt(const BSFixedString &variableName, SInt32 &out);		// 11 00650C30
	virtual bool	GetAnimationVariableBool(const BSFixedString &variableName, bool &out);			// 12 00650C20

	MEMBER_FN_PREFIX(IAnimationGraphManagerHolder);
	DEFINE_MEMBER_FN(SetAnimationVariableBool, bool, 0x00650C60, const BSFixedString &variableName, bool val);
	DEFINE_MEMBER_FN(SetAnimationVariableInt, bool, 0x00650C80, const BSFixedString &variableName, SInt32 val);
	DEFINE_MEMBER_FN(SetAnimationVariableFloat, bool, 0x00650CA0, const BSFixedString &variableName, float val);

//	void	** _vtbl;
};

// 128
class IPostAnimationChannelUpdateFunctor
{
public:
	virtual ~IPostAnimationChannelUpdateFunctor();
//	void	** _vtbl;
};

// 54
class TESObjectREFR : public TESForm
{
public:
	enum { kTypeID = kFormType_Reference };

	enum
	{
		kFlag_Harvested = 0x2000,
	};

	// currently none of these have been analyzed
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual void	Unk_4A(void);
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	GetStartingPos(NiPoint3 * pos);
	virtual void	Unk_52(void);
	virtual void	Unk_53(void);
	virtual void	Unk_54(void);
	virtual void	Unk_55(void);
	virtual void	Unk_56(void);
	virtual void	Unk_57(void);
	virtual void	Unk_58(void);
	virtual void	Unk_59(void);
	virtual void	Unk_5A(void);
	virtual void	GetMarkerPosition(NiPoint3 * pos);
	virtual void	Unk_5C(void);
	virtual void	Unk_5D(void);
	virtual void	Unk_5E(void);
	virtual void	Unk_5F(void);
	virtual void	Unk_60(void);
	virtual BSFaceGenNiNode *	GetFaceGenNiNode(void);
	virtual void	Unk_62(void);
	virtual BSFaceGenAnimationData *	GetFaceGenAnimationData(void);
	virtual void	Unk_64(void);
	virtual void	Unk_65(void);
	virtual void	Unk_66(void);
	virtual void	Unk_67(void);
	virtual void	Unk_68(void);
	virtual void	Unk_69(void);
	virtual void	Unk_6A(void);
	virtual void 	Unk_6B(void);
	virtual void	Unk_6C(void);
	virtual void	SetNiNode(NiNode * root, UInt32 unk1); // NULL, 1?
	virtual void	Unk_6E(void);
	virtual NiNode	* GetNiRootNode(UInt32 firstPerson);
	virtual NiNode	* GetNiNode(void);	// Root of the skeleton (Get3D)
	virtual void	Unk_71(void);
	virtual void	Unk_72(void);
	virtual void	Unk_73(void);
	virtual void	Unk_74(void);
	virtual void	Unk_75(void);
	virtual void	Unk_76(void);
	virtual void	Unk_77(void);
	virtual void	Unk_78(void);
	virtual void	Unk_79(void);
	virtual void	Unk_7A(void);
	virtual void	Unk_7B(void);
	virtual void	Unk_7C(void);
	virtual void	Unk_7D(void);
	virtual ActorWeightModel	* GetWeightModel(UInt32 weightModel); // 0 Small 1 Large
	virtual void	Unk_7F(void);
	virtual void	Unk_80(void);
	virtual void	Unk_81(void);
	virtual void	Unk_82(void);
	virtual void	Unk_83(void);
	virtual void	Unk_84(void);
	virtual void	Unk_85(void);
	virtual void	Unk_86(void);
	virtual void	Unk_87(void);
	virtual void	Unk_88(void);
	virtual void	ResetInventory(bool unk);
	virtual void	Unk_8A(void);
	virtual void	Unk_8B(void);
	virtual void	Unk_8C(void);
	virtual void	Unk_8D(void);
	virtual void	Unk_8E(void);
	virtual void	Unk_8F(void);
	virtual void	Unk_90(void);
	virtual void	Unk_91(void);
	virtual void	Unk_92(void);
	virtual void	Unk_93(void);
	virtual void	Unk_94(void);
	virtual void	Unk_95(void);
	virtual void	Unk_96(void);
	virtual void	Unk_97(void);
	virtual void	Unk_98(void);
	virtual bool	IsDead(UInt8 unk1); // unk1 = 1 for Actors
	virtual void	Unk_9A(void);
	virtual void	Unk_9B(void);

	struct LoadedState
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		NiNode	* node;	// 20
		// ... probably more
	};

	// parents
	BSHandleRefObject	handleRefObject;	// 14
	BSTEventSink <BSAnimationGraphEvent>	animGraphEventSink;	// 1C
	IAnimationGraphManagerHolder			animGraphHolder;	// 20

	// members
	TESForm* baseForm;	// 24
	NiPoint3	rot;	// 28
	NiPoint3	pos;	// 34

	TESObjectCELL	* parentCell;	// 40
	LoadedState		* loadedState;	// 44
	BaseExtraList	extraData;		// 48
	UInt32			unk50;			// flags?

	UInt32 CreateRefHandle(void);

	MEMBER_FN_PREFIX(TESObjectREFR);
	DEFINE_MEMBER_FN(GetBaseScale, float, 0x004D5230);
	DEFINE_MEMBER_FN(IsOffLimits, bool, 0x004DA760);
	DEFINE_MEMBER_FN(GetWeight, float, 0x004EA180);
	DEFINE_MEMBER_FN(GetReferenceName, const char *, 0x004DE820);
	DEFINE_MEMBER_FN(GetWorldspace, TESWorldSpace*, 0x4D5EB0);
};

STATIC_ASSERT(sizeof(TESObjectREFR) == 0x54);
STATIC_ASSERT(offsetof(TESObjectREFR, handleRefObject) == 0x14);

// 04
class IMovementInterface
{
public:
	virtual ~IMovementInterface();
//	void	** _vtbl;	// 00
};

// 04
class IMovementState : public IMovementInterface
{
public:
	virtual ~IMovementState();
};

// 0C
class ActorState : public IMovementState
{
public:
	virtual ~ActorState();

//	void	** _vtbl;	// 00

	// older versions of this class stored flags in a UInt64
	// this forced the addition of 4 useless padding bytes
	// current and future versions store flags as two UInt32s

	enum {
		kState_Running = 0x40,
		kState_Walking = 0x80,
		kState_Sprinting = 0x100,
		kState_Sneaking = 0x200,
		kState_Swimming = 0x400,
		kState_AttackState = 0xF0000000
	};

	UInt32	flags04;
	UInt32	flags08;

	bool IsWeaponDrawn()
	{
		return (flags08 >> 5 & 7) >= 3;
	}
};

//STATIC_ASSERT(sizeof(ActorState) == 0x10);

class MagicItem;
class MagicCaster {
public:
	virtual ~MagicCaster();								// 00660D60

	virtual void	InitMagicCaster(MagicItem *arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6, UInt32 arg7);	// 01 00662040
	virtual UInt32	MagicCaster_Unk_02(void);						// 02 0065F4E0
	virtual void	MagicCaster_Unk_03(void);						// 03 0065EEE0
	virtual bool	MagicCaster_Unk_04(void);						// 04 0065FDF0
	virtual void	MagicCaster_Unk_05(void);						// 05 0065E2E0
	virtual void	MagicCaster_Unk_06(void);						// 06 00662420
	virtual void	MagicCaster_Unk_07(void);						// 07 006C50E0 { return; }
	virtual void	MagicCaster_Unk_08(UInt32 arg1);				// 08 00588F30 { return; }
	virtual void	MagicCaster_Unk_09(UInt32 arg1, UInt32 arg2, UInt32 arg3);		// 09 00D62BE0 { return; }
	virtual bool	MagicCaster_Unk_0A(UInt32 arg1, UInt32 arg2, float* arg3, UInt32* arg4, UInt32 arg5, UInt32 arg6, UInt32 arg7);	// 0A 0065E0F0
																																	//				{ if (arg3) { *arg3 = 1.0; } if (arg4) { *arg4 = 0; } }
	virtual Actor *	MagicCaster_Unk_0B(void);						// 0B 005EADD0 { return nullptr; }
	virtual Actor *	GetCaster();									// 0C 005EADD0 { return nullptr; }
	virtual Actor *	MagicCaster_Unk_0D(Actor** arg1);				// 0D 0065E120
	virtual UInt32	MagicCaster_Unk_0E(void);						// 0E 005EADD0 { return nullptr; }
	virtual void	MagicCaster_Unk_0F(void);						// 0F 006C50E0 { return; }
	virtual void	MagicCaster_Unk_10(MagicItem *arg);				// 10 00588F30 { return; }
	virtual void	MagicCaster_Unk_11(void);						// 11 006C50E0 { return; }
	virtual void	MagicCaster_Unk_12(void);						// 12 006C50E0 { return; }
	virtual void	MagicCaster_Unk_13(void);						// 13 006C50E0 { return; }
	virtual void	CalculateMagickaCost(void);						// 14 0065E7A0
	virtual UInt32	GetSlot(void);									// 15 005EADD0 { return 0; }
	virtual bool	MagicCaster_Unk_16(void);						// 16 0092D110 { return false; }
	virtual void	MagicCaster_Unk_17(bool b);						// 17 00588F30 { return; }
	virtual void	MagicCaster_Unk_18(UInt32 arg1);				// 18 0065FBB0
	virtual void	MagicCaster_Unk_19(UInt32 arg1);				// 19 0065FB20
	virtual void	MagicCaster_Unk_1A(UInt32 arg1);				// 1A 0065F290
	virtual void	MagicCaster_Unk_1B(UInt32 arg1, UInt32 arg2);	// 1B 0065EC70
	virtual void	MagicCaster_Unk_1C(UInt32 arg1, UInt32 arg2, UInt32 arg3);		// 1C 0065E0D0

																					// @members
																					//void				** _vtbl;		// 00 - 010C95B4
	tArray<void *>	unk04;			// 04
	UInt32			unk10;			// 10 - init'd to 0 | maybe RefHandle
	MagicItem			* magicItem;	// 14 - init'd to 0
	UInt32				unk18;			// 18 - init'd to 0 | current state? 1=cast start 7=???
	float				castTime;		// 1C - init'd to 0
	float				cost;			// 20 - init'd to 1.0f
	UInt32				unk24;			// 24 - init'd to 0 | temporary init'd arg6 in InitMagicCaster()
	float				unk28;			// 28 - init'd to 0 | 3E800000 = 0.25f
	UInt32				unk2C;			// 2C 0

										// ctor		0065FD90
};
STATIC_ASSERT(sizeof(MagicCaster) == 0x30);

// 19C
class Actor : public TESObjectREFR
{
public:
	enum { kTypeID = kFormType_Character };

	virtual ~Actor();
	virtual void Unk_9C(void);
	virtual void Unk_9D(void);
	virtual void Unk_9E(void);
	virtual void Unk_9F(void);
	virtual void Unk_A0(void);
	virtual void Unk_A1(void);
	virtual void Unk_A2(void);
	virtual void Unk_A3(void);
	virtual void Unk_A4(void);
	virtual void DrawSheatheWeapon(bool draw);
	virtual void Unk_A6(void);
	virtual void Unk_A7(void);
	virtual void Unk_A8(void);
	virtual void Unk_A9(void);
	virtual void Unk_AA(void);
	virtual void Unk_AB(void);
	virtual void Unk_AC(void);
	virtual void Unk_AD(void);
	virtual void Unk_AE(void);
	virtual void Unk_AF(void);
	virtual void Unk_B0(void);
	virtual void Unk_B1(void);
	virtual void Unk_B2(void);
	virtual void Unk_B3(void);
	virtual void Unk_B4(void);
	virtual void Unk_B5(void);
	virtual void Unk_B6(void);
	virtual void Unk_B7(void);
	virtual void Unk_B8(void);
	virtual void Unk_B9(void);
	virtual void Unk_BA(void);
	virtual void Unk_BB(void);
	virtual void Unk_BC(void);
	virtual void Unk_BD(void);
	virtual void Unk_BE(void);
	virtual void Unk_BF(void);
	virtual void Unk_C0(void);
	virtual void Unk_C1(void);
	virtual void Unk_C2(void);
	virtual void Unk_C3(void);
	virtual void Unk_C4(void);
	virtual void Unk_C5(void);
	virtual void Unk_C6(void);
	virtual void Unk_C7(void);
	virtual void Unk_C8(void);
	virtual void Unk_C9(void);
	virtual void Unk_CA(void);
	virtual void Unk_CB(void);
	virtual void Unk_CC(void);
	virtual void Unk_CD(void);
	virtual void Unk_CE(void);
	virtual void Unk_CF(void);
	virtual void Unk_D0(void);
	virtual void Unk_D1(void);
	virtual void Unk_D2(void);
	virtual void Unk_D3(void);
	virtual void Unk_D4(void);
	virtual void Unk_D5(void);
	virtual void Unk_D6(void);
	virtual void Unk_D7(void);
	virtual void Unk_D8(void);
	virtual void Unk_D9(void);
	virtual void Unk_DA(void);
	virtual void Unk_DB(void);
	virtual void Unk_DC(void);
	virtual void Unk_DD(void);
	virtual void Unk_DE(void);
	virtual void Unk_DF(void);
	virtual void Unk_E0(void);
	virtual void Unk_E1(void);
	virtual bool	IsInCombat(void);								// 006E1340
	virtual void	Unk_E3(void);									// 006E2BF0 PlayerCharacter={ return; } 
	virtual void	Unk_E4(void);									// 006E5510
	virtual float	Unk_E5(void);									// 0066E8A0 { return 0.0f; }
	virtual float	Unk_E6(void);									// 0066E8A0 { return 0.0f; }
	virtual void	Unk_E7(void);									// 005EADD0 { return 0; }
	virtual void	Unk_E8(void);									// 0092D110 { return false; }
	virtual void	Unk_E9(UInt32 arg);								// 004091A0 { return false; }
	virtual void	Unk_EA(void);									// 006C5FA0
	virtual void	Unk_EB(void);									// 006AB7D0
	virtual void	Unk_EC(void);									// 006AB850
	virtual void	Unk_ED(void);									// 006C5F90
	virtual void	OnWeaponSwing(void);							// 006E3D50
	virtual void Unk_EF(void);
	virtual void Unk_F0(void);
	virtual void Unk_F1(void);
	virtual void Unk_F2(void);
	virtual void Unk_F3(void);
	virtual void Unk_F4(void);
	virtual void Unk_F5(void);
	virtual void AdvanceSkill(UInt32 skillId, float points, UInt32 unk1, UInt32 unk2);
	virtual void Unk_F7(void);
	virtual void Unk_F8(void);
	virtual void VisitPerks(void); // BGSPerk::FindPerkInRanksVisitor
	virtual void AddPerk(BGSPerk * perk, UInt32 unk1);
	virtual void RemovePerk(BGSPerk * perk);

	// 0C
	class SpellArray
	{
	public:
		SpellItem * Get(UInt32 idx)
		{
			if(idx >= spellCount) return NULL;

			if(allocatedCount & kLocalAlloc)
				return (idx == 0) ? singleSpell : NULL;
			else
				return spells ? spells[idx] : NULL;
		}

		UInt32	Length(void)	{ return spellCount; }

	private:
		enum
	{
			kLocalAlloc = 0x80000000,
		};

		UInt32		allocatedCount;		// 00 - upper bit is set when single-element optimization is used
		union {
		SpellItem	** spells;		// 04
			SpellItem	* singleSpell;	// 04 - used when kLocalAlloc is set
		};
		UInt32		spellCount;		// 08
	};

	enum Flags1 {
		kFlags_AIEnabled = 0x02,
		kFlags_IsPlayerTeammate = 0x4000000
	};
	struct Flags2 {
		bool			unk00 : 1;					// 00 0x00000001
		bool			hasInteraction : 1;			// 01 0x00000002
		unsigned char	unk02 : 5;					// 02 0x0000007C
		bool			canDoFavor : 1;				// 07 0x00000080
		unsigned char	unk08 : 4;					// 08 0x00000F00
		bool			trespassing : 1;			// 0C 0x00001000
		bool			unk0C : 1;					// 0D 0x00002000
		bool			killMove : 1;				// 0E 0x00004000
		bool			attackedByAllActors : 1;	// 0F 0x00008000 - see Actor.SetAttackActorOnSight
		bool			commandedActor : 1;			// 10 0x00010000 - see Actor.IsCommandedActor
		bool			unk11 : 1;					// 11 0x00020000
		bool			essential : 1;				// 12 0x00040000
	};

	enum SlotType {
		kSlot_LeftHand = 0,
		kSlot_RightHand,
		kSlot_Unknown,
		kSlot_PowerOrShout,

		kNumSlots
	};

	MagicTarget		magicTarget;					// 054
	ActorValueOwner	actorValueOwner;				// 060
	ActorState		actorState;						// 064
	BSTEventSink<void*>	transformDeltaEvent;		// 070 .?AV?$BSTEventSink@VBSTransformDeltaEvent@@@@
	BSTEventSink<void*>	characterMoveFinishEvent;	// 074 .?AV?$BSTEventSink@VbhkCharacterMoveFinishEvent@@@@
	IPostAnimationChannelUpdateFunctor	unk078;		// 078 IPostAnimationChannelUpdateFunctor
	UInt32	flags1;									// 07C
	UInt32	unk080;									// 080
	UInt32	unk084;									// 084
	ActorProcessManager	* processManager;			// 088
	UInt32	unk08C[(0x0FC - 0x08C) >> 2];			// 08C
	SpellArray	addedSpells;						// 0FC
	MagicCaster					* magicCaster[kNumSlots];			// 108 [0] lefthand [1] right hand [2] unknown [3] power/shout
	MagicItem					* equippingMagicItems[kNumSlots];	// 118
	TESForm	* equippedShout;						// 128
	UInt32	unk12C;									// 12C
	TESRace			* race;							// 130
	UInt32	unk134;									// 134
	Flags2	flags2;									// 138
	UInt32	unk13C[(0x19C - 0x13C) >> 2];

	TESForm * GetEquippedObject(bool abLeftHand);

	MEMBER_FN_PREFIX(Actor);
	DEFINE_MEMBER_FN(QueueNiNodeUpdate, void, 0x00730EE0, bool updateWeight);
	DEFINE_MEMBER_FN(HasPerk, bool, 0x006AA190, BGSPerk * perk);
	DEFINE_MEMBER_FN(GetLevel, UInt16, 0x006A7320);
	DEFINE_MEMBER_FN(SetRace, void, 0x006AF590, TESRace*, bool isPlayer);


	DEFINE_MEMBER_FN(UpdateWeaponAbility, void, 0x006ED980, TESForm*, BaseExtraList * extraData, bool bLeftHand);
	DEFINE_MEMBER_FN(UpdateArmorAbility, void, 0x006E8650, TESForm*, BaseExtraList * extraData);

	bool IsOnMount() const;

	void UpdateSkinColor();
	void UpdateHairColor();
};

STATIC_ASSERT(offsetof(Actor, magicTarget) == 0x54);
STATIC_ASSERT(offsetof(Actor, actorValueOwner) == 0x60);
STATIC_ASSERT(offsetof(Actor, actorState) == 0x64);
STATIC_ASSERT(offsetof(Actor, transformDeltaEvent) == 0x70);
STATIC_ASSERT(offsetof(Actor, addedSpells) == 0xFC);
STATIC_ASSERT(sizeof(Actor) == 0x19C);

// 19C
class Character : public Actor
{
public:
	enum { kTypeID = kFormType_Character };
};

STATIC_ASSERT(sizeof(Character) == 0x19C);

// 720
class PlayerCharacter : public Character
{
public:
	virtual ~PlayerCharacter();

	// parents
	BSTEventSink <void *>	menuOpenCloseEvent;			// 19C .?AV?$BSTEventSink@VMenuOpenCloseEvent@@@@
	BSTEventSink <void *>	menuModeChangeEvent;		// 1A0 .?AV?$BSTEventSink@VMenuModeChangeEvent@@@@
	BSTEventSink <void *>	userEventEnabledEvent;		// 1A4 .?AV?$BSTEventSink@VUserEventEnabledEvent@@@@
	BSTEventSource <void *>	actorCellEventSource;		// 1A8 .?AV?$BSTEventSource@UBGSActorCellEvent@@@@
	BSTEventSource <void *>	actorDeathEventSource;		// 1D8 .?AV?$BSTEventSource@UBGSActorDeathEvent@@@@
	BSTEventSource <void *>	positionPlayerEventSource;	// 208 .?AV?$BSTEventSource@UPositionPlayerEvent@@@@

	UInt32	pad238[(0x490 - 0x238) >> 2];	// 238
	UInt32	unk490;							// 490 - Handle
	UInt32	pad494[(0x568 - 0x494) >> 2];	// 494
	UInt32	unk568;							// 568 - Handle
	UInt32	pad56C[(0x58C - 0x56C) >> 2];	// 56C
	NiNode	* firstPersonSkeleton;			// 58C
	UInt32	pad590[(0x5AC - 0x590) >> 2];
	UInt32	lastRiddenHorseHandle;			// 5AC - Handle
	UInt32	pad5B0[(0x614 - 0x5B0) >> 2];
	PlayerSkills *	skills;					// 614
	UInt32	targetHandle;					// 618
	UInt32	unk61C;							// 61C
	UInt32	unk620;							// 620
	UInt32	unk624;							// 624
	UInt32	unk628;							// 628
	UInt32	unk62C;							// 62C
	tArray<UInt32>	hostileHandles;			// 630
	UInt32	unk63C;							// 63C
	UInt32	unk640;							// 640
	TESForm	* tempPoison;					// 644
	UInt32	numTeammates;					// 648
	UInt32	pad64C[(0x6E0 - 0x64C) >> 2];
	UInt8	unk6E0;							// 6E0
	UInt8	numPerkPoints;					// 6E1
	UInt16  unk6E2;							// 6E2
	UInt32	unk6E4;							// 6E4

	tArray <TintMask *>	tintMasks;			// 6E8		// These are the actual tints
	tArray <TintMask *>	* overlayTintMasks;	// 6F4		// These apply when overlay head parts is enabled

	// Overlayed tints should be the same as original tints
	// occasionally they can have no type so index matching
	// is required to set anything on the tint
	TintMask * GetOverlayTintMask(TintMask * original);

	// Confirmed - Same as ExtraContainerChanges::EntryData
	// This type is used by scaleform to extend data
	// It can be used to extend more of the "ExtraData"
	struct ObjDesc
	{
		TESForm					* form;
		tList<BaseExtraList>	* extraData;
		SInt32					countDelta;

		MEMBER_FN_PREFIX(ObjDesc);
		DEFINE_MEMBER_FN(GenerateName, const char *, 0x00475AA0);
	};

	MEMBER_FN_PREFIX(PlayerCharacter);
	DEFINE_MEMBER_FN(GetTintList, tArray <TintMask *> *, 0x0055FF90);
	DEFINE_MEMBER_FN(GetNumTints, UInt32, 0x00735F40, UInt32 tintType);
	DEFINE_MEMBER_FN(GetTintMask, TintMask *, 0x00735F00, UInt32 tintType, UInt32 index);
	DEFINE_MEMBER_FN(GetDamage, double, 0x00730810, ObjDesc * pForm);
	DEFINE_MEMBER_FN(GetArmorValue, double, 0x007307E0, ObjDesc * pForm);
};

STATIC_ASSERT(offsetof(PlayerCharacter, userEventEnabledEvent) == 0x1A4);
STATIC_ASSERT(offsetof(PlayerCharacter, numPerkPoints) == 0x6E1);
STATIC_ASSERT(offsetof(PlayerCharacter, tintMasks) == 0x6E8);
STATIC_ASSERT(offsetof(PlayerCharacter, overlayTintMasks) == 0x6F4);
STATIC_ASSERT(offsetof(PlayerCharacter, unk568) == 0x568);
STATIC_ASSERT(offsetof(PlayerCharacter, lastRiddenHorseHandle) == 0x5AC);
STATIC_ASSERT(offsetof(PlayerCharacter, skills) == 0x614);
STATIC_ASSERT(offsetof(PlayerCharacter, tempPoison) == 0x644);
STATIC_ASSERT(offsetof(PlayerCharacter, hostileHandles) == 0x630);

// D8
class Explosion : public TESObjectREFR
{
	//
};

// 118
class ChainExplosion : public Explosion
{
	//
};

// 88
class Hazard : public TESObjectREFR
{
	enum { kTypeID = kFormType_Hazard };
};

// 140+
class Projectile : public TESObjectREFR
{
	//
};

// 150
class BarrierProjectile : public Projectile
{
	enum { kTypeID = kFormType_BarrierProj };
};

// 174
class BeamProjectile : public Projectile
{
	enum { kTypeID = kFormType_BeamProj };
};

// 16C
class ConeProjectile : public Projectile
{
	enum { kTypeID = kFormType_ConeProj };
};

// 148
class FlameProjectile : public Projectile
{
	enum { kTypeID = kFormType_FlameProj };
};

// 148
class GrenadeProjectile : public Projectile
{
	enum { kTypeID = kFormType_Grenade };
};

// 148
class MissileProjectile : public Projectile
{
	enum { kTypeID = kFormType_Missile };
};

// 150
class ArrowProjectile : public MissileProjectile
{
	enum { kTypeID = kFormType_Arrow };
};

// This does alot more, but no idea what it is yet
// ??
class CrosshairRefHandleHolder
{
	UInt32	unk00;					// 00
	UInt32	crosshairRefHandle;		// 04
	// ...

public:
	static CrosshairRefHandleHolder * GetSingleton(void)
	{
		return *((CrosshairRefHandleHolder **)0x01B115D8);
	}

	UInt32 CrosshairRefHandle(void) const		{ return crosshairRefHandle; }
};
