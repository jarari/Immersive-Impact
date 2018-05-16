Scriptname BingleQuickSwitchMCM extends SKI_ConfigBase  

; SCRIPT VERSION ----------------------------------------------------------------------------------

int function GetVersion()
	return 2
endFunction


; PRIVATE VARIABLES -------------------------------------------------------------------------------

; OIDs (T:Text B:Toggle S:Slider M:Menu, C:Color, K:Key)
int	switchKeyOID_K
int sliderDamage1H2HOID_S
int sliderDamage2H1HOID_S
int sliderSpeed1H2HOID_S
int sliderSpeed2H1HOID_S

; Public
int property switchkeyCode auto
BingleQuickSwitch property pBingleQuickSwitch auto
float property damage1h2h auto
float property damage2h1h auto
float property speed1h2h auto
float property speed2h1h auto

; Private

; Function
Function UpdateMultiplier(int type, float val) global native

Function SyncMultipliers()
	UpdateMultiplier(0, damage1h2h)
	UpdateMultiplier(1, damage2h1h)
	UpdateMultiplier(2, speed1h2h)
	UpdateMultiplier(3, speed2h1h)
EndFunction

; INITIALIZATION ----------------------------------------------------------------------------------
Function UpdateV2()
	damage1h2h = 1.0
	damage2h1h = 1.0
	speed1h2h = 1.2
	speed2h1h = 0.9
EndFunction

; @implements SKI_ConfigBase
event OnConfigInit()
	Pages = new string[2]
	Pages[0] = "$BINGLE_QS_PAGE_SETTINGS"
	
	ModName = "Quick Switch Hands"
	
	switchkeyCode = 0x31
	pBingleQuickSwitch.SwitchKeyRegister(switchkeyCode)
	UpdateV2()
endEvent

; @implements SKI_QuestBase
event OnVersionUpdate(int a_version)
	if(a_version >= 2 && CurrentVersion < 2)
		UpdateV2()
	endif
endEvent


; EVENTS ------------------------------------------------------------------------------------------
; @implements SKI_ConfigBase
event OnPageReset(string a_page)
	UnloadCustomContent()
	if(a_page == "$BINGLE_QS_PAGE_SETTINGS")
		SetCursorFillMode(TOP_TO_BOTTOM)
		switchKeyOID_K = AddKeyMapOption("$BINGLE_QS_SETTINGS_KEY", switchkeyCode)
		
		SetCursorPosition(1)
		AddHeaderOption("$BINGLE_QS_SETTINGS_DAMAGE_HEADER")
		sliderDamage1H2HOID_S = AddSliderOption("$BINGLE_QS_SETTINGS_1H2H", damage1h2h, "x {2}")
		sliderDamage2H1HOID_S = AddSliderOption("$BINGLE_QS_SETTINGS_2H1H", damage2h1h, "x {2}")
		
		AddHeaderOption("$BINGLE_QS_SETTINGS_SPEED_HEADER")
		sliderSpeed1H2HOID_S = AddSliderOption("$BINGLE_QS_SETTINGS_1H2H", speed1h2h, "x {2}")
		sliderSpeed2H1HOID_S = AddSliderOption("$BINGLE_QS_SETTINGS_2H1H", speed2h1h, "x {2}")
		
		SyncMultipliers()
	endif
endEvent

Event OnOptionHighlight(int option)
	if(option == sliderDamage1H2HOID_S || option == sliderDamage2H1HOID_S)
		SetInfoText("$BINGLE_QS_SETTINGS_DAMAGE_DESC")
	elseif(option == sliderSpeed1H2HOID_S || option == sliderSpeed2H1HOID_S)
		SetInfoText("$BINGLE_QS_SETTINGS_SPEED_DESC")
	else
		SetInfoText("")
	endif
EndEvent

event OnOptionKeyMapChange(int option, int keyCode, string conflictControl, string conflictName)
	if (option == switchKeyOID_K)
		bool continue = true
		if (conflictControl != "")
			string msg
			if (conflictName != "")
				msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
			else
				msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
			endIf

			continue = ShowMessage(msg, true, "$Yes", "$No")
		endIf

		if (continue)
			switchkeyCode = keyCode
			SetKeymapOptionValue(switchKeyOID_K, keyCode)
			pBingleQuickSwitch.SwitchKeyRegister(keyCode)
		endIf
	endIf
endEvent

string function GetCustomControl(int keyCode)
	if (keyCode == switchkeyCode)
		return "Switch holding hands"
	else
		return ""
	endIf
endFunction

event OnOptionSliderOpen(int option)
	SetSliderDialogInterval(0.05)
	SetSliderDialogRange(0.1, 2.5)
	if(option == sliderDamage1H2HOID_S)
		SetSliderDialogStartValue(damage1h2h)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderDamage2H1HOID_S)
		SetSliderDialogStartValue(damage2h1h)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderSpeed1H2HOID_S)
		SetSliderDialogStartValue(speed1h2h)
		SetSliderDialogDefaultValue(1.2)
		
	elseif(option == sliderSpeed2H1HOID_S)
		SetSliderDialogStartValue(speed2h1h)
		SetSliderDialogDefaultValue(0.9)
		
	endif
endEvent

event OnOptionSliderAccept(int option, float value)
	if(option == sliderDamage1H2HOID_S)
		damage1h2h = value
		SetSliderOptionValue(sliderDamage1H2HOID_S, damage1h2h, "x {2}")
		UpdateMultiplier(0, damage1h2h)
		
	elseif(option == sliderDamage2H1HOID_S)
		damage2h1h = value
		SetSliderOptionValue(sliderDamage2H1HOID_S, damage2h1h, "x {2}")
		UpdateMultiplier(1, damage2h1h)
		
	elseif(option == sliderSpeed1H2HOID_S)
		speed1h2h = value
		SetSliderOptionValue(sliderSpeed1H2HOID_S, speed1h2h, "x {2}")
		UpdateMultiplier(2, speed1h2h)
		
	elseif(option == sliderSpeed2H1HOID_S)
		speed2h1h = value
		SetSliderOptionValue(sliderSpeed2H1HOID_S, speed2h1h, "x {2}")
		UpdateMultiplier(3, speed2h1h)
	
	endif
endEvent