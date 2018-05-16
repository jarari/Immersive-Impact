Scriptname BingleQuickSwitch extends ReferenceAlias

BingleQuickSwitchMCM property pBingleQuickSwitchMCM auto
Actor property player auto
Message property BingleEquipTypeDialogue auto
Weapon property Unarmed auto
Weapon lastWeapon
Weapon lastLeftWeapon
Equipslot ohEQ
Equipslot thEQ
int lastChoice
int lastLeftChoice
bool drawing
float lastSwitchTime

Function UnregisterForEquipRequest() native
Function RegisterForEquipRequest() native
int Function SwitchHandsShort(Weapon wep) global native
Function SwitchHands(Weapon wep, int _2h, int reEquip) global native
Function RevertHands(Weapon wep) global native

Function SwitchKeyRegister(int keyCode)
	UnregisterForAllKeys()
	RegisterForKey(keyCode)
EndFunction

Function InitializeStuff()
	lastSwitchTime = 0
	player = Game.GetPlayer()
	UnregisterForEquipRequest()
	RegisterForEquipRequest()
	UnregisterForActorAction(7)
	UnregisterForActorAction(8)
	UnregisterForActorAction(9)
	UnregisterForActorAction(10)
	RegisterForActorAction(7)
	RegisterForActorAction(8)
	RegisterForActorAction(9)
	RegisterForActorAction(10)
	ohEQ = Game.GetForm(0x00013F44) As EquipSlot
	thEQ = Game.GetForm(0x00013F45) As EquipSlot
	SwitchKeyRegister(pBingleQuickSwitchMCM.switchkeyCode)
	pBingleQuickSwitchMCM.SyncMultipliers()
EndFunction

Event OnInit()
	lastChoice = -1
EndEvent

Event OnPlayerLoadGame()
	InitializeStuff()
	if(lastWeapon)
		int weptype = player.GetEquippedWeapon(false).GetWeaponType()
		if(weptype >= 5 && lastChoice == 0)
			lastChoice = 1
		elseif(weptype < 5 && lastChoice == 1)
			lastChoice = 0
		endif
		player.SheatheWeapon()
		SwitchHands(lastWeapon, lastChoice, 1)
	endif
	if(lastLeftWeapon)
		int weptype = player.GetEquippedWeapon(false).GetWeaponType()
		if(weptype >= 5 && lastLeftChoice == 0)
			lastLeftChoice = 1
		elseif(weptype < 5 && lastLeftChoice == 1)
			lastLeftChoice = 0
		endif
		SwitchHands(lastLeftWeapon, lastLeftChoice, 0)
	endif
EndEvent

Event OnLoad()
	InitializeStuff()
EndEvent

Event OnEquipRequested(ObjectReference _ac, Form wep, int choice, int dummy)
	Utility.WaitMenuMode(0.25)
	lastWeapon = wep as Weapon
	lastChoice = choice
	(_ac as Actor).EquipItem(wep, false, true)
	Utility.WaitMenuMode(0.15)
	if(lastWeapon == wep)
		RevertHands(lastWeapon)
		(_ac as Actor).DrawWeapon()
	endif
EndEvent

Event OnActorAction(int actionType, Actor akActor, Form source, int slot)
	if((actionType == 7 || actionType == 9) && akActor.GetEquippedWeapon(false) && akActor.GetEquippedWeapon(false) == lastWeapon && !drawing)
		RevertHands(lastWeapon)
		drawing = true
	elseif((actionType == 8 || actionType == 10) && akActor.GetEquippedWeapon(false) && akActor.GetEquippedWeapon(false) == lastWeapon && drawing)
		SwitchHands(lastWeapon, lastChoice, 0)
		drawing = false
	endif
EndEvent

Event OnKeyDown(int keyCode)
	if(keyCode == pBingleQuickSwitchMCM.switchkeyCode && !drawing && player.IsWeaponDrawn() && Utility.GetCurrentRealTime() - lastSwitchTime >= 5)
		int result = SwitchHandsShort(player.GetEquippedWeapon(false))
		if(result == 1)
			drawing = true
			lastSwitchTime = Utility.GetCurrentRealTime()
		endif
	endif
EndEvent

Event OnObjectEquipped(Form akBaseObject, ObjectReference akReference)
	if(akBaseObject as Weapon && akBaseObject != lastWeapon && akBaseObject == player.GetEquippedWeapon(false))
		int weptype = (akBaseObject as Weapon).GetWeaponType()
		if(weptype >= 1 && weptype <= 6)
			lastWeapon = akBaseObject as Weapon
			drawing = false
			if(weptype >= 5)
				lastChoice = 1
			else
				lastChoice = 0
			endif
			Utility.WaitMenuMode(0.05)
			RevertHands(lastWeapon)
		endif
	elseif(akBaseObject as Weapon && akBaseObject == lastWeapon && akBaseObject == player.GetEquippedWeapon(false) && !drawing)
		int weptype = (akBaseObject as Weapon).GetWeaponType()
		if(weptype >= 5 && lastChoice == 0)
			lastChoice = 1
		elseif(weptype < 5 && lastChoice == 1)
			lastChoice = 0
		endif
		Utility.WaitMenuMode(0.05)
		RevertHands(lastWeapon)
	elseif(akBaseObject as Weapon && akBaseObject == player.GetEquippedWeapon(true))
		lastLeftWeapon = akBaseObject as Weapon
		int weptype = (akBaseObject as Weapon).GetWeaponType()
		if(weptype >= 5)
			lastLeftChoice = 1
		elseif(weptype < 5)
			lastLeftChoice = 0
		endif
	endif
EndEvent

Event OnObjectUnequipped(Form akBaseObject, ObjectReference akReference)
	if(akBaseObject == lastWeapon)
		player.UnequipItem(lastWeapon, false, true)
	endif
EndEvent