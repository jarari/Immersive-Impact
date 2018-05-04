Scriptname BingleImmersiveFeedbackMCM extends SKI_ConfigBase  

; SCRIPT VERSION ----------------------------------------------------------------------------------

int function GetVersion()
	return 4
endFunction


; PRIVATE VARIABLES -------------------------------------------------------------------------------

; OIDs (T:Text B:Toggle S:Slider M:Menu, C:Color, K:Key)
int	sliderPreAttackSpeedOID_S
int sliderSwingSpeedOID_S
int sliderSwingSpeed1HOID_S
int sliderSwingSpeedDaggerOID_S
int sliderSwingSpeedFistOID_S
int sliderBaseSpeedOID_S

int toggleMovementRestrainOID_B

; Public
float property valuePreAttackSpeed auto
float property valueSwingSpeed auto
float property valueSwingSpeed1H auto
float property valueSwingSpeedDagger auto
float property valueSwingSpeedFist auto
float property valueBaseSpeed auto
bool property shouldRestrain auto
int property IFState auto

; Private
float defaultPreAttackSpeed
float defaultSwingSpeed
float defaultSwingSpeed1H
float defaultSwingSpeedDagger
float defaultSwingSpeedFist
float defaultBaseSpeed
bool defaultRestrainOption

; Function
Function EnableRestrainMovement() global native
Function DisableRestrainMovement() global native

; INITIALIZATION ----------------------------------------------------------------------------------

Function UpdateV2()
	defaultSwingSpeed1H = 1.5
	defaultSwingSpeedDagger = 1.0
	valueSwingSpeed1H = defaultSwingSpeed1H
	valueSwingSpeedDagger = defaultSwingSpeedDagger
	;Debug.Notification("1H "+defaultSwingSpeed1H+" Dag "+defaultSwingSpeedDagger)
EndFunction

Function UpdateV3()
	defaultSwingSpeed = 1.75
	defaultBaseSpeed = 0.9
	valueSwingSpeed = defaultSwingSpeed
	valueBaseSpeed = defaultBaseSpeed
EndFunction

Function UpdateV4()
	defaultSwingSpeedFist = 1.25
	valueSwingSpeedFist = defaultSwingSpeedFist
	defaultRestrainOption = false
	shouldRestrain = defaultRestrainOption
EndFunction

; @implements SKI_ConfigBase
event OnConfigInit()
	Pages = new string[1]
	Pages[0] = "$BINGLE_PAGE_SETTINGS"
	
	ModName = "Immersive Impact"
	defaultPreAttackSpeed = 0.8
	defaultSwingSpeed = 2.0
	defaultBaseSpeed = 1.25
	
	valuePreAttackSpeed = defaultPreAttackSpeed
	valueSwingSpeed = defaultSwingSpeed
	valueBaseSpeed = defaultBaseSpeed
	IFState = 0
	UpdateV2()
	UpdateV3()
	UpdateV4()
endEvent

; @implements SKI_QuestBase
event OnVersionUpdate(int a_version)
	if(a_version >= 2 && CurrentVersion < 2)
		UpdateV2()
	endif
	
	if(a_version >= 3 && CurrentVersion < 3)
		UpdateV3()
		Game.GetPlayer().ForceAV("weaponSpeedMult", 1)
		Game.GetPlayer().ForceAV("LeftWeaponSpeedMult", 1)
		if(CurrentVersion == 2)
			Debug.Messagebox("Swing speed for 2H weapons and post-attack speed has been reset. Please check the settings.")
		endif
	endif
	
	if(a_version >= 4 && CurrentVersion < 3)
		UpdateV4()
	endif
endEvent

Function UpdateIFState(int s)
	IFState = s
EndFunction


; EVENTS ------------------------------------------------------------------------------------------

; @implements SKI_ConfigBase
event OnPageReset(string a_page)
	UnloadCustomContent()
	if(a_page == "$BINGLE_PAGE_SETTINGS")
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddHeaderOption("$BINGLE_PAGE_SETTINGS_SPEEDMUL")
		sliderPreAttackSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_PREATTACK", valuePreAttackSpeed, "x {2}")
		sliderSwingSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWING", valueSwingSpeed, "x {2}")
		sliderSwingSpeed1HOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWING1H", valueSwingSpeed1H, "x {2}")
		sliderSwingSpeedDaggerOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWINGDAGGER", valueSwingSpeedDagger, "x {2}")
		sliderSwingSpeedFistOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWINGFIST", valueSwingSpeedFist, "x {2}")
		sliderBaseSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_POSTATTACK", valueBaseSpeed, "x {2}")
		toggleMovementRestrainOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_RESTRAINMOVEMENT", shouldRestrain)
		if(shouldRestrain)
			EnableRestrainMovement()
		else
			DisableRestrainMovement()
		endif
		string stateText = "$BINGLE_IF_NOTINIT"
		int opt = OPTION_FLAG_DISABLED
		if(IFState == 1)
			stateText = "$BINGLE_IF_WORKING"
		elseif(IFState == 2)
			stateText = "$BINGLE_IF_INIT"
		else
			opt = OPTION_FLAG_NONE
		endif
		AddTextOptionST("StateText", "", stateText, opt)
		SetCursorPosition(1)
	endif
endEvent

event OnOptionSelect(int option)
	if (option == toggleMovementRestrainOID_B)
		shouldRestrain = !shouldRestrain
		SetToggleOptionValue(toggleMovementRestrainOID_B, shouldRestrain)
		if(shouldRestrain)
			EnableRestrainMovement()
		else
			DisableRestrainMovement()
		endif
	endIf
endEvent

event OnOptionSliderOpen(int option)
	SetSliderDialogInterval(0.05)
	SetSliderDialogRange(0.1, 2.5)
	if(option == sliderPreAttackSpeedOID_S)
		SetSliderDialogStartValue(valuePreAttackSpeed)
		SetSliderDialogDefaultValue(defaultPreAttackSpeed)
		
	elseif(option == sliderSwingSpeedOID_S)
		SetSliderDialogStartValue(valueSwingSpeed)
		SetSliderDialogDefaultValue(defaultSwingSpeed)
		
	elseif(option == sliderSwingSpeed1HOID_S)
		SetSliderDialogStartValue(valueSwingSpeed1H)
		SetSliderDialogDefaultValue(defaultSwingSpeed1H)
		
	elseif(option == sliderSwingSpeedDaggerOID_S)
		SetSliderDialogStartValue(valueSwingSpeedDagger)
		SetSliderDialogDefaultValue(defaultSwingSpeedDagger)
		
	elseif(option == sliderSwingSpeedFistOID_S)
		SetSliderDialogStartValue(valueSwingSpeedFist)
		SetSliderDialogDefaultValue(defaultSwingSpeedFist)
		
	elseif(option == sliderBaseSpeedOID_S)
		SetSliderDialogStartValue(valueBaseSpeed)
		SetSliderDialogDefaultValue(defaultBaseSpeed)
	
	endif
endEvent

event OnOptionSliderAccept(int option, float value)
	if(option == sliderPreAttackSpeedOID_S)
		valuePreAttackSpeed = value
		SetSliderOptionValue(sliderPreAttackSpeedOID_S, valuePreAttackSpeed, "x {2}")
		
	elseif(option == sliderSwingSpeedOID_S)
		valueSwingSpeed = value
		SetSliderOptionValue(sliderSwingSpeedOID_S, valueSwingSpeed, "x {2}")
		
	elseif(option == sliderSwingSpeed1HOID_S)
		valueSwingSpeed1H = value
		SetSliderOptionValue(sliderSwingSpeed1HOID_S, valueSwingSpeed1H, "x {2}")
		
	elseif(option == sliderSwingSpeedDaggerOID_S)
		valueSwingSpeedDagger = value
		SetSliderOptionValue(sliderSwingSpeedDaggerOID_S, valueSwingSpeedDagger, "x {2}")
		
	elseif(option == sliderSwingSpeedFistOID_S)
		valueSwingSpeedFist = value
		SetSliderOptionValue(sliderSwingSpeedFistOID_S, valueSwingSpeedFist, "x {2}")
		
	elseif(option == sliderBaseSpeedOID_S)
		valueBaseSpeed = value
		SetSliderOptionValue(sliderBaseSpeedOID_S, valueBaseSpeed, "x {2}")
	endif
endEvent