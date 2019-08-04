Scriptname BingleImmersiveFeedback extends ReferenceAlias

BingleImmersiveFeedbackMCM property pMCMScript auto
Weapon property Unarmed auto

Function RegisterForFistRequest() native
Function RegisterForInitRequest() native
Function RegisterForNotificationRequest() native
Function RegisterForMessageBoxRequest() native
Function EvaluateEvent(string eventname) global native
Function RegisterForConfigRequest() native
Function RegisterForTranslateToRequest() native

Actor player
Function InitializeStuff()
	pMCMScript.UpdateIFState(1)
	player = GetActorReference()
	RegisterForFistRequest()
	RegisterForNotificationRequest()
	RegisterForMessageBoxRequest()
	RegisterForConfigRequest()
	RegisterForTranslateToRequest()
	UnregisterForAnimationEvent(player, "preHitFrame")
	UnregisterForAnimationEvent(player, "weaponSwing")
	UnregisterForAnimationEvent(player, "weaponLeftSwing")
	UnregisterForAnimationEvent(player, "AttackWinStart")
	UnregisterForAnimationEvent(player, "AttackWinStartLeft")
	UnregisterForAnimationEvent(player, "AttackWinEnd")
	UnregisterForAnimationEvent(player, "AttackWinEndLeft")
	bool a = RegisterForAnimationEvent(player, "preHitFrame")
	bool b = RegisterForAnimationEvent(player, "weaponSwing")
	bool c = RegisterForAnimationEvent(player, "weaponLeftSwing")
	bool d = RegisterForAnimationEvent(player, "AttackWinStart")
	bool e = RegisterForAnimationEvent(player, "AttackWinStartLeft")
	bool f = RegisterForAnimationEvent(player, "AttackWinEnd")
	bool g = RegisterForAnimationEvent(player, "AttackWinEndLeft")
	if(a && b && c && d && e && f && g)
		pMCMScript.UpdateIFState(2)
	endif
EndFunction

Event OnInit()
	pMCMScript.UpdateIFState(0)
	RegisterForInitRequest()
EndEvent

Event OnPlayerLoadGame()
	RegisterForInitRequest()
EndEvent

Event OnSyncConfig(int type, float v)
	pMCMScript.SyncConfig(type, v)
EndEvent

Event OnInitializeRequested(int dummy)
	InitializeStuff()
EndEvent

Event OnFistRequested(ObjectReference _ac)
	(_ac as Actor).EquipItem(Unarmed, false, true)
EndEvent

Event OnNotificationRequest(string content)
	Debug.Notification(content)
EndEvent

Event OnMessageBoxRequest(string content)
	Debug.MessageBox(content)
EndEvent

Event OnTranslateTo(float x, float y, float z, float vel)
	player.TranslateTo(x, y, z, player.GetAngleX(), player.GetAngleY(), player.GetAngleZ(), vel, 0)
EndEvent

Event OnAnimationEvent(ObjectReference akSource, string asEventName)
	EvaluateEvent(asEventName)
	;Debug.Notification(asEventName + " fired.")
	;if(asEventName == "preHitFrame")
	;	(akSource as Actor).ForceAV("weaponSpeedMult", pMCMScript.valuePreAttackSpeed)
	;elseif(asEventName == "weaponSwing")
	;	weptype = (akSource as Actor).GetEquippedWeapon(false).GetWeaponType()
	;	if(weptype == 5 || weptype == 6)
	;		(akSource as Actor).ForceAV("weaponSpeedMult", pMCMScript.valueSwingSpeed)
	;	elseif(weptype == 0 || weptype == 1 || weptype == 3 || weptype == 4)
	;		(akSource as Actor).ForceAV("weaponSpeedMult", pMCMScript.valueSwingSpeed1H)
	;	elseif(weptype == 2)
	;		(akSource as Actor).ForceAV("weaponSpeedMult", pMCMScript.valueSwingSpeedDagger)
	;	endif
	;elseif(asEventName == "AttackWinStart")
	;	(akSource as Actor).ForceAV("weaponSpeedMult", pMCMScript.valueBaseSpeed)
	;elseif(asEventName == "weaponLeftSwing")
	;	weptype = (akSource as Actor).GetEquippedWeapon(true).GetWeaponType()
	;	if(weptype == 5 || weptype == 6)
	;		(akSource as Actor).ForceAV("LeftWeaponSpeedMult", pMCMScript.valueSwingSpeed)
	;	elseif(weptype == 0 || weptype == 1 || weptype == 3 || weptype == 4)
	;		(akSource as Actor).ForceAV("LeftWeaponSpeedMult", pMCMScript.valueSwingSpeed1H)
	;	elseif(weptype == 2)
	;		(akSource as Actor).ForceAV("LeftWeaponSpeedMult", pMCMScript.valueSwingSpeedDagger)
	;	endif
	;endif
EndEvent