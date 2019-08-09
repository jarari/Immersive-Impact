Scriptname BingleImmersiveFeedbackMCM extends SKI_ConfigBase  

; SCRIPT VERSION ----------------------------------------------------------------------------------

int function GetVersion()
	return 6
endFunction


; PRIVATE VARIABLES -------------------------------------------------------------------------------

; OIDs (T:Text B:Toggle S:Slider M:Menu, C:Color, K:Key)
int	sliderPreAttackSpeedOID_S
int sliderSwingSpeedOID_S
int sliderSwingSpeed1HOID_S
int sliderSwingSpeedDaggerOID_S
int sliderSwingSpeedFistOID_S
int sliderBaseSpeedOID_S
int sliderCustomLOID_S
int sliderCustomROID_S
int sliderChargeMulOID_S

int toggleRestrainMovementOID_B
int toggleAimHelperOID_B

; Public
BingleImmersiveFeedback property pBingleImmersiveFeedback auto
float property valuePreAttackSpeed auto
float property valueSwingSpeed auto
float property valueSwingSpeed1H auto
float property valueSwingSpeedDagger auto
float property valueSwingSpeedFist auto
float property valueBaseSpeed auto
float property valueCustomL auto
float property valueCustomR auto
float property valueChargeMul auto
bool property valueRestrainMovement auto
bool property valueAimHelper auto
int property IFState auto

; Private
float defaultPreAttackSpeed
float defaultSwingSpeed
float defaultSwingSpeed1H
float defaultSwingSpeedDagger
float defaultSwingSpeedFist
float defaultBaseSpeed
bool defaultRestrainMovement
bool defaultAimHelper

; Function
Function UpdateConfig(int formId, int ctype, float v) global native
Function UpdateSaveConfig(int formId, int ctype, float v) global native
Function SaveConfig() global native


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
	defaultRestrainMovement = false
EndFunction

Function UpdateV5()
EndFunction

Function UpdateV6()
	valueRestrainMovement = defaultRestrainMovement
	
	defaultAimHelper = false
	valueAimHelper = defaultAimHelper
EndFunction

; @implements SKI_ConfigBase
event OnConfigInit()
	Pages = new string[2]
	Pages[0] = "$BINGLE_PAGE_SETTINGS"
	Pages[1] = "$BINGLE_PAGE_CUSTOMWEAP"
	
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
	UpdateV6()
endEvent

; @implements SKI_QuestBase
event OnVersionUpdate(int a_version)
	if(a_version >= 2 && CurrentVersion < 2)
		UpdateV2()
	endif
	
	if(a_version >= 3 && CurrentVersion < 3)
		UpdateV3()
	endif
	
	if(a_version >= 4 && CurrentVersion < 4)
		UpdateV4()
	endif
	
	if(a_version >= 6 && CurrentVersion < 6)
		UpdateV6()
	endif
endEvent

Function UpdateIFState(int s)
	IFState = s
EndFunction


; EVENTS ------------------------------------------------------------------------------------------

Function SyncConfig(int type, float v)
	if(type == 2)
		valuePreAttackSpeed = v
	elseif(type == 3)
		valueSwingSpeed1H = v
	elseif(type == 4)
		valueSwingSpeed = v
	elseif(type == 5)
		valueSwingSpeedDagger = v
	elseif(type == 6)
		valueSwingSpeedFist = v
	elseif(type == 7)
		valueBaseSpeed = v
	elseif(type == 8)
		valueCustomL = v
	elseif(type == 9)
		valueCustomR = v
	elseif(type == 10)
		if(v == 1)
			valueRestrainMovement = true
		else
			valueRestrainMovement = false
		endif
	elseif(type == 11)
		if(v == 1)
			valueAimHelper = true
		else
			valueAimHelper = false
		endif
	elseif(type == 12)
		valueChargeMul = v
	endif
EndFunction

; @implements SKI_ConfigBase
event OnPageReset(string a_page)
	UnloadCustomContent()
	if(a_page == "$BINGLE_PAGE_SETTINGS")
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddHeaderOption("$BINGLE_PAGE_SETTINGS_SPEEDMUL")
		sliderPreAttackSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_PREATTACK", valuePreAttackSpeed, "x {2}")
		sliderSwingSpeed1HOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWING1H", valueSwingSpeed1H, "x {2}")
		sliderSwingSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWING", valueSwingSpeed, "x {2}")
		sliderSwingSpeedDaggerOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWINGDAGGER", valueSwingSpeedDagger, "x {2}")
		sliderSwingSpeedFistOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWINGFIST", valueSwingSpeedFist, "x {2}")
		sliderBaseSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_POSTATTACK", valueBaseSpeed, "x {2}")
		toggleRestrainMovementOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_RESTRAINMOVEMENT", valueRestrainMovement)
		toggleAimHelperOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_AIMHELPER", valueAimHelper)
		sliderChargeMulOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_CHARGEMUL", valueChargeMul, "x {2}")
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
	elseif(a_page == "$BINGLE_PAGE_CUSTOMWEAP")
		Actor p = Game.GetPlayer()
		int flags = OPTION_FLAG_NONE
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddHeaderOption(p.GetEquippedWeapon(true).GetName())
		if(p.GetEquippedWeapon(true) == None)
			flags = OPTION_FLAG_DISABLED
		endif
		sliderCustomLOID_S = AddSliderOption("$BINGLE_PAGE_CUSTOMWEAPL_SWING", valueCustomL, "x {2}", flags)
		
		
		SetCursorPosition(1)
		AddHeaderOption(p.GetEquippedWeapon(false).GetName())
		if(p.GetEquippedWeapon(false) != None)
			flags = OPTION_FLAG_NONE
		endif
		sliderCustomROID_S = AddSliderOption("$BINGLE_PAGE_CUSTOMWEAPR_SWING", valueCustomR, "x {2}", flags)
	endif
endEvent

event OnOptionSelect(int option)
	if (option == toggleRestrainMovementOID_B)
		valueRestrainMovement = !valueRestrainMovement
		SetToggleOptionValue(toggleRestrainMovementOID_B, valueRestrainMovement)
		if(valueRestrainMovement)
			UpdateSaveConfig(0, 10, 1)
		else
			UpdateSaveConfig(0, 10, 0)
		endif
	elseif (option == toggleAimHelperOID_B)
		valueAimHelper = !valueAimHelper
		SetToggleOptionValue(toggleAimHelperOID_B, valueAimHelper)
		if(valueAimHelper)
			UpdateSaveConfig(0, 11, 1)
		else
			UpdateSaveConfig(0, 11, 0)
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
		SetSliderDialogDefaultValue(defaultSwingSpeedFist)
		
	elseif(option == sliderCustomLOID_S)
		SetSliderDialogStartValue(valueCustomL)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderCustomROID_S)
		SetSliderDialogStartValue(valueCustomR)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderChargeMulOID_S)
		SetSliderDialogStartValue(valueChargeMul)
		SetSliderDialogDefaultValue(1.0)
	
	endif
endEvent

event OnOptionSliderAccept(int option, float value)
	if(option == sliderPreAttackSpeedOID_S)
		valuePreAttackSpeed = value
		SetSliderOptionValue(sliderPreAttackSpeedOID_S, valuePreAttackSpeed, "x {2}")
		UpdateSaveConfig(0, 2, valuePreAttackSpeed)
		
	elseif(option == sliderSwingSpeed1HOID_S)
		valueSwingSpeed1H = value
		SetSliderOptionValue(sliderSwingSpeed1HOID_S, valueSwingSpeed1H, "x {2}")
		UpdateSaveConfig(0, 3, valueSwingSpeed1H)
		
	elseif(option == sliderSwingSpeedOID_S)
		valueSwingSpeed = value
		SetSliderOptionValue(sliderSwingSpeedOID_S, valueSwingSpeed, "x {2}")
		UpdateSaveConfig(0, 4, valueSwingSpeed)
		
	elseif(option == sliderSwingSpeedDaggerOID_S)
		valueSwingSpeedDagger = value
		SetSliderOptionValue(sliderSwingSpeedDaggerOID_S, valueSwingSpeedDagger, "x {2}")
		UpdateSaveConfig(0, 5, valueSwingSpeedDagger)
		
	elseif(option == sliderSwingSpeedFistOID_S)
		valueSwingSpeedFist = value
		SetSliderOptionValue(sliderSwingSpeedFistOID_S, valueSwingSpeedFist, "x {2}")
		UpdateSaveConfig(0, 6, valueSwingSpeedFist)
		
	elseif(option == sliderBaseSpeedOID_S)
		valueBaseSpeed = value
		SetSliderOptionValue(sliderBaseSpeedOID_S, valueBaseSpeed, "x {2}")
		UpdateSaveConfig(0, 7, valueBaseSpeed)
		
	elseif(option == sliderCustomLOID_S)
		valueCustomL = value
		SetSliderOptionValue(sliderCustomLOID_S, valueCustomL, "x {2}")
		UpdateSaveConfig(Game.GetPlayer().GetEquippedWeapon(true).GetFormID(), 8, valueCustomL)
		
	elseif(option == sliderCustomROID_S)
		valueCustomR = value
		SetSliderOptionValue(sliderCustomROID_S, valueCustomR, "x {2}")
		UpdateSaveConfig(Game.GetPlayer().GetEquippedWeapon(false).GetFormID(), 9, valueCustomR)
		
	elseif(option == sliderChargeMulOID_S)
		valueChargeMul = value
		SetSliderOptionValue(sliderChargeMulOID_S, valueChargeMul, "x {2}")
		UpdateSaveConfig(0, 12, valueChargeMul)
		
	endif
endEvent