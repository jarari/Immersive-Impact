Scriptname BingleImmersiveFeedback extends ReferenceAlias

BingleImmersiveFeedbackMCM property pMCMScript auto
Weapon property Unarmed auto

Function RegisterForFistRequest() native
Function RegisterForInitRequest() native
Function RegisterForNotificationRequest() native
Function RegisterForMessageBoxRequest() native
Function EvaluateTypes(string eventname, float pre, float swing1h, float swing2h, float swingdag, float swingfist, float post) global native
Function RestrainMovement(Actor a, int restrain) global native

Actor player
Function InitializeStuff()
	pMCMScript.UpdateIFState(1)
	player = GetActorReference()
	UnregisterForAnimationEvent(player, "preHitFrame")
	UnregisterForAnimationEvent(player, "weaponSwing")
	UnregisterForAnimationEvent(player, "AttackWinStart")
	UnregisterForAnimationEvent(player, "weaponLeftSwing")
	bool a = RegisterForAnimationEvent(player, "preHitFrame")
	bool b = RegisterForAnimationEvent(player, "weaponSwing")
	bool c = RegisterForAnimationEvent(player, "AttackWinStart")
	bool d = RegisterForAnimationEvent(player, "weaponLeftSwing")
	if(a && b && c && d)
		pMCMScript.UpdateIFState(2)
	endif
EndFunction

Event OnInit()
	pMCMScript.UpdateIFState(0)
	RegisterForInitRequest()
	RegisterForFistRequest()
	RegisterForNotificationRequest()
	RegisterForMessageBoxRequest()
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

Event OnAnimationEvent(ObjectReference akSource, string asEventName)
	EvaluateTypes(asEventName, pMCMScript.valuePreAttackSpeed, pMCMScript.valueSwingSpeed1H, pMCMScript.valueSwingSpeed, pMCMScript.valueSwingSpeedDagger, pMCMScript.valueSwingSpeedFist, pMCMScript.valueBaseSpeed)
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