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
int sliderDeflectChanceMulOID_S
int sliderDeflectChanceMaxOID_S
int sliderStaggerResetTimeOID_S
int sliderStaggerLimitOID_S
int sliderStaggerDamageMaxOID_S

int toggleSpeedAdjustmentOID_B
int toggleRestrainMovementOID_B
int toggleAimHelperOID_B
int toggleHitFeedbackOID_B
int toggleStaggerAnyOID_B
int resetQuestOID_B

; Public
Quest property _BingleImmersiveFeedbackHelper auto
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
float property valueDeflectChanceMul auto
float property valueDeflectChanceMax auto
float property valueStaggerResetTime auto
float property valueStaggerLimit auto
float property valueStaggerDamageMax auto
bool property valueSpeedAdjustment auto
bool property valueRestrainMovement auto
bool property valueAimHelper auto
bool property valueHitFeedback auto
bool property valueStaggerAny auto
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
	elseif(type == 13)
		if(v == 1)
			valueHitFeedback = true
		else
			valueHitFeedback = false
		endif
	elseif(type == 14)
		if(v == 1)
			valueSpeedAdjustment = true
		else
			valueSpeedAdjustment = false
		endif
	elseif(type == 15)
		valueDeflectChanceMul = v * 100
	elseif(type == 16)
		valueDeflectChanceMax = v
	elseif(type == 17)
		valueStaggerResetTime = v
	elseif(type == 18)
		valueStaggerLimit = v
	elseif(type == 19)
		valueStaggerDamageMax = v
	elseif(type == 20)
		if(v == 1)
			valueStaggerAny = true
		else
			valueStaggerAny = false
		endif
	endif
EndFunction

; @implements SKI_ConfigBase
event OnPageReset(string a_page)
	UnloadCustomContent()
	if(a_page == "$BINGLE_PAGE_SETTINGS")
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddHeaderOption("$BINGLE_PAGE_SETTINGS_SPEEDMUL")
		toggleSpeedAdjustmentOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_ENABLESPEED", valueSpeedAdjustment)
		sliderPreAttackSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_PREATTACK", valuePreAttackSpeed, "x {2}")
		sliderSwingSpeed1HOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWING1H", valueSwingSpeed1H, "x {2}")
		sliderSwingSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWING", valueSwingSpeed, "x {2}")
		sliderSwingSpeedDaggerOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWINGDAGGER", valueSwingSpeedDagger, "x {2}")
		sliderSwingSpeedFistOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_SWINGFIST", valueSwingSpeedFist, "x {2}")
		sliderBaseSpeedOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_POSTATTACK", valueBaseSpeed, "x {2}")
		SetCursorPosition(1)
		AddHeaderOption("$BINGLE_PAGE_SETTINGS_FEATURES")
		toggleRestrainMovementOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_RESTRAINMOVEMENT", valueRestrainMovement)
		toggleAimHelperOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_AIMHELPER", valueAimHelper)
		sliderChargeMulOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_CHARGEMUL", valueChargeMul, "x {2}")
		toggleHitFeedbackOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_HITFEEDBACK", valueHitFeedback)
		sliderDeflectChanceMulOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_DeflectChanceMul", valueDeflectChanceMul, "{1}% of armor value")
		sliderDeflectChanceMaxOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_DeflectChanceMax", valueDeflectChanceMax, "{1}%")
		sliderStaggerResetTimeOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_StaggerResetTime", valueStaggerResetTime, "{2} sec(s)")
		sliderStaggerLimitOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_StaggerLimit", valueStaggerLimit, "{0} hit(s)")
		sliderStaggerDamageMaxOID_S = AddSliderOption("$BINGLE_PAGE_SETTINGS_StaggerDamageMax", valueStaggerDamageMax, "{0}")
		toggleStaggerAnyOID_B = AddToggleOption("$BINGLE_PAGE_SETTINGS_STAGGERANY", valueStaggerAny)
		;resetQuestOID_B = AddTextOption("", "$BINGLE_PAGE_SETTINGS_RESETQUEST", OPTION_FLAG_NONE)
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

event OnOptionHighlight(int option)
	if (option == sliderDeflectChanceMulOID_S)
		SetInfoText("$BINGLE_PAGE_DESC_DeflectChanceMul")
	elseif (option == sliderDeflectChanceMaxOID_S)
		SetInfoText("$BINGLE_PAGE_DESC_DeflectChanceMax")
	elseif (option == sliderStaggerResetTimeOID_S)
		SetInfoText("$BINGLE_PAGE_DESC_StaggerResetTime")
	elseif (option == sliderStaggerLimitOID_S)
		SetInfoText("$BINGLE_PAGE_DESC_StaggerLimit")
	elseif (option == sliderStaggerDamageMaxOID_S)
		SetInfoText("$BINGLE_PAGE_DESC_StaggerDamageMax")
	elseif (option == toggleStaggerAnyOID_B)
		SetInfoText("$BINGLE_PAGE_DESC_StaggerAny")
	else
		SetInfoText("")
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
		
	elseif (option == toggleAimHelperOID_B)
		valueAimHelper = !valueAimHelper
		SetToggleOptionValue(toggleAimHelperOID_B, valueAimHelper)
		if(valueAimHelper)
			UpdateSaveConfig(0, 11, 1)
		else
			UpdateSaveConfig(0, 11, 0)
		endif
		
	elseif (option == toggleHitFeedbackOID_B)
		valueHitFeedback = !valueHitFeedback
		SetToggleOptionValue(toggleHitFeedbackOID_B, valueHitFeedback)
		if(valueHitFeedback)
			UpdateSaveConfig(0, 13, 1)
		else
			UpdateSaveConfig(0, 13, 0)
		endif
		
	elseif (option == resetQuestOID_B)
		SetOptionFlags(option, OPTION_FLAG_DISABLED)
		Utility.Wait(1)
		Debug.Notification("$BINGLE_QUESTRESET_STOP")
		_BingleImmersiveFeedbackHelper.Stop()
		Utility.Wait(1)
		Debug.Notification("$BINGLE_QUESTRESET_RESET")
		_BingleImmersiveFeedbackHelper.Reset()
		(_BingleImmersiveFeedbackHelper.GetAlias(0) as ReferenceAlias).ForceRefTo(Game.GetPlayer())
		(_BingleImmersiveFeedbackHelper.GetAlias(1) as ReferenceAlias).ForceRefTo(Game.GetPlayer())
		Utility.Wait(1)
		Debug.Notification("$BINGLE_QUESTRESET_START")
		_BingleImmersiveFeedbackHelper.Start()
		
	elseif (option == toggleSpeedAdjustmentOID_B)
		valueSpeedAdjustment = !valueSpeedAdjustment
		SetToggleOptionValue(toggleSpeedAdjustmentOID_B, valueSpeedAdjustment)
		if(valueSpeedAdjustment)
			UpdateSaveConfig(0, 14, 1)
		else
			UpdateSaveConfig(0, 14, 0)
		endif
		
	elseif (option == toggleStaggerAnyOID_B)
		valueStaggerAny = !valueStaggerAny
		SetToggleOptionValue(toggleStaggerAnyOID_B, valueStaggerAny)
		if(valueStaggerAny)
			UpdateSaveConfig(0, 20, 1)
		else
			UpdateSaveConfig(0, 20, 0)
		endif
	endIf
endEvent

event OnOptionSliderOpen(int option)
	SetSliderDialogInterval(0.05)
	SetSliderDialogRange(0.1, 3.0)
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
		SetSliderDialogInterval(0.05)
		SetSliderDialogRange(0.1, 1.25)
		SetSliderDialogStartValue(valueBaseSpeed)
		SetSliderDialogDefaultValue(defaultSwingSpeedFist)
		
	elseif(option == sliderCustomLOID_S)
		SetSliderDialogStartValue(valueCustomL)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderCustomROID_S)
		SetSliderDialogStartValue(valueCustomR)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderChargeMulOID_S)
		SetSliderDialogInterval(0.01)
		SetSliderDialogRange(0.1, 3)
		SetSliderDialogStartValue(valueChargeMul)
		SetSliderDialogDefaultValue(1.0)
		
	elseif(option == sliderDeflectChanceMulOID_S)
		SetSliderDialogInterval(0.1)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valueDeflectChanceMul)
		SetSliderDialogDefaultValue(10)
		
	elseif(option == sliderDeflectChanceMaxOID_S)
		SetSliderDialogInterval(0.1)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valueDeflectChanceMax)
		SetSliderDialogDefaultValue(50.0)
		
	elseif(option == sliderStaggerResetTimeOID_S)
		SetSliderDialogInterval(0.01)
		SetSliderDialogRange(1.0, 5)
		SetSliderDialogStartValue(valueStaggerResetTime)
		SetSliderDialogDefaultValue(2.0)
		
	elseif(option == sliderStaggerLimitOID_S)
		SetSliderDialogInterval(1)
		SetSliderDialogRange(1, 10)
		SetSliderDialogStartValue(valueStaggerLimit)
		SetSliderDialogDefaultValue(3)
		
	elseif(option == sliderStaggerDamageMaxOID_S)
		SetSliderDialogInterval(1)
		SetSliderDialogRange(25, 1000)
		SetSliderDialogStartValue(valueStaggerDamageMax)
		SetSliderDialogDefaultValue(200)
	
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
		
	elseif(option == sliderDeflectChanceMulOID_S)
		valueDeflectChanceMul = value
		SetSliderOptionValue(sliderDeflectChanceMulOID_S, valueDeflectChanceMul, "{1}% of armor value")
		UpdateSaveConfig(0, 15, valueDeflectChanceMul / 100.0)
		
	elseif(option == sliderDeflectChanceMaxOID_S)
		valueDeflectChanceMax = value
		SetSliderOptionValue(sliderDeflectChanceMaxOID_S, valueDeflectChanceMax, "{1}%")
		UpdateSaveConfig(0, 16, valueDeflectChanceMax)
		
	elseif(option == sliderStaggerResetTimeOID_S)
		valueStaggerResetTime = value
		SetSliderOptionValue(sliderStaggerResetTimeOID_S, valueStaggerResetTime, "{2} sec(s)")
		UpdateSaveConfig(0, 17, valueStaggerResetTime)
		
	elseif(option == sliderStaggerLimitOID_S)
		valueStaggerLimit = value
		SetSliderOptionValue(sliderStaggerLimitOID_S, valueStaggerLimit, "{0} hit(s)")
		UpdateSaveConfig(0, 18, valueStaggerLimit)
		
	elseif(option == sliderStaggerDamageMaxOID_S)
		valueStaggerDamageMax = value
		SetSliderOptionValue(sliderStaggerDamageMaxOID_S, valueStaggerDamageMax, "{0}")
		UpdateSaveConfig(0, 19, valueStaggerDamageMax)
		
	endif
endEvent