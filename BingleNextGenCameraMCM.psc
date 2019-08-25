Scriptname BingleNextGenCameraMCM extends SKI_ConfigBase  

; SCRIPT VERSION ----------------------------------------------------------------------------------

int function GetVersion()
	return 1
endFunction


; PRIVATE VARIABLES -------------------------------------------------------------------------------

; OIDs (T:Text B:Toggle S:Slider M:Menu, C:Color, K:Key)
int	slideriTickRateOID_S
int	sliderfCamSmoothOID_S
int	sliderfIdleOffsetXOID_S
int	sliderfIdleOffsetZOID_S
int	sliderfCombatOffsetXOID_S
int	sliderfCombatOffsetZOID_S
int	sliderfMagicOffsetXOID_S
int	sliderfMagicOffsetZOID_S
int	sliderfBowOffsetXOID_S
int	sliderfBowOffsetZOID_S
int	sliderfHorseIdleOffsetXOID_S
int	sliderfHorseIdleOffsetZOID_S
int	sliderfHorseCombatOffsetXOID_S
int	sliderfHorseCombatOffsetZOID_S
int	sliderfHorseMagicOffsetX_S
int	sliderfHorseMagicOffsetZOID_S
int	sliderfHorseBowOffsetXOID_S
int	sliderfHorseBowOffsetZOID_S
int	sliderfCamVelocityStrengthXOID_S
int	sliderfCamVelocityStrengthYOID_S
int	sliderfCamVelocityStrengthZOID_S
int	sliderfSneakIdleOffsetXOID_S
int	sliderfSneakIdleOffsetZOID_S
int	sliderfSneakCombatOffsetXOID_S
int	sliderfSneakCombatOffsetZOID_S
int	sliderfSneakMagicOffsetX_S
int	sliderfSneakMagicOffsetZOID_S
int	sliderfSneakBowOffsetXOID_S
int	sliderfSneakBowOffsetZOID_S
int	keyiShoulderSwitchOID_K

; Public
float property valueiTickRate auto
float property valuefCamSmooth auto
float property valuefIdleOffsetX auto
float property valuefIdleOffsetZ auto
float property valuefCombatOffsetX auto
float property valuefCombatOffsetZ auto
float property valuefMagicOffsetX auto
float property valuefMagicOffsetZ auto
float property valuefBowOffsetX auto
float property valuefBowOffsetZ auto
float property valuefHorseIdleOffsetX auto
float property valuefHorseIdleOffsetZ auto
float property valuefHorseCombatOffsetX auto
float property valuefHorseCombatOffsetZ auto
float property valuefHorseMagicOffsetX auto
float property valuefHorseMagicOffsetZ auto
float property valuefHorseBowOffsetX auto
float property valuefHorseBowOffsetZ auto
float property valuefCamVelocityStrengthX auto
float property valuefCamVelocityStrengthY auto
float property valuefCamVelocityStrengthZ auto
float property valuefSneakIdleOffsetX auto
float property valuefSneakIdleOffsetZ auto
float property valuefSneakCombatOffsetX auto
float property valuefSneakCombatOffsetZ auto
float property valuefSneakMagicOffsetX auto
float property valuefSneakMagicOffsetZ auto
float property valuefSneakBowOffsetX auto
float property valuefSneakBowOffsetZ auto
int property keyiShoulderSwitch auto
int property valueiShoulder auto

; Private
float defaultiTickRate
float defaultfCamSmooth
float defaultfIdleOffsetX
float defaultfIdleOffsetZ
float defaultfCombatOffsetX
float defaultfCombatOffsetZ
float defaultfMagicOffsetX
float defaultfMagicOffsetZ
float defaultfBowOffsetX
float defaultfBowOffsetZ
float defaultfHorseIdleOffsetX
float defaultfHorseIdleOffsetZ
float defaultfHorseCombatOffsetX
float defaultfHorseCombatOffsetZ
float defaultfHorseMagicOffsetX
float defaultfHorseMagicOffsetZ
float defaultfHorseBowOffsetX
float defaultfHorseBowOffsetZ
float defaultfCamVelocityStrengthX
float defaultfCamVelocityStrengthY
float defaultfCamVelocityStrengthZ
float defaultfSneakIdleOffsetX
float defaultfSneakIdleOffsetZ
float defaultfSneakCombatOffsetX
float defaultfSneakCombatOffsetZ
float defaultfSneakMagicOffsetX
float defaultfSneakMagicOffsetZ
float defaultfSneakBowOffsetX
float defaultfSneakBowOffsetZ
int defaultiShoulderSwitch
int defaultiShoulder

; Function
Function RegisterScript() native
Function UpdateConfig(int ctype, float v) global native
Function UpdateSaveConfig(int ctype, float v) global native
Function SaveConfig() global native
Function ForceSyncConfig() global native

Function ShoulderSwitchKeyRegister(int keyCode)
	UnregisterForAllKeys()
	RegisterForKey(keyCode)
EndFunction

Function UpdateV1()
	Pages = new string[2]
	Pages[0] = "$BINGLE_NGC_PAGE_SETTINGS"
	Pages[1] = "$BINGLE_NGC_PAGE_EXPERIMENTAL"
EndFunction


; INITIALIZATION ----------------------------------------------------------------------------------

; @implements SKI_ConfigBase
event OnConfigInit()
	Pages = new string[2]
	Pages[0] = "$BINGLE_NGC_PAGE_SETTINGS"
	Pages[1] = "$BINGLE_NGC_PAGE_EXPERIMENTAL"
	ModName = "$BINGLE_NGC_NAME"
	RegisterScript()
	ForceSyncConfig()
endEvent

event OnGameReload()
	parent.OnGameReload()
	RegisterScript()
	ForceSyncConfig()
endEvent

; @implements SKI_QuestBase
event OnVersionUpdate(int a_version)
	if(a_version >= 1 && CurrentVersion < 1)
		UpdateV1()
	endif
endEvent


; EVENTS ------------------------------------------------------------------------------------------

event OnSyncConfig(int type, float v)
	if(type == 0)
		valueiTickRate = v
	elseif(type == 1)
		valuefCamSmooth = v
	elseif(type == 2)
		valuefIdleOffsetX = v
	elseif(type == 3)
		valuefIdleOffsetZ = v
	elseif(type == 4)
		valuefCombatOffsetX = v
	elseif(type == 5)
		valuefCombatOffsetZ = v
	elseif(type == 6)
		valuefMagicOffsetX = v
	elseif(type == 7)
		valuefMagicOffsetZ = v
	elseif(type == 8)
		valuefBowOffsetX = v
	elseif(type == 9)
		valuefBowOffsetZ = v
	elseif(type == 10)
		valuefHorseIdleOffsetX = v
	elseif(type == 11)
		valuefHorseIdleOffsetZ = v
	elseif(type == 12)
		valuefHorseCombatOffsetX = v
	elseif(type == 13)
		valuefHorseCombatOffsetZ = v
	elseif(type == 14)
		valuefHorseMagicOffsetX = v
	elseif(type == 15)
		valuefHorseMagicOffsetZ = v
	elseif(type == 16)
		valuefHorseBowOffsetX = v
	elseif(type == 17)
		valuefHorseBowOffsetZ = v
	elseif(type == 18)
		valuefCamVelocityStrengthX = v
	elseif(type == 19)
		valuefSneakIdleOffsetX = v
	elseif(type == 20)
		valuefSneakIdleOffsetZ = v
	elseif(type == 21)
		valuefSneakCombatOffsetX = v
	elseif(type == 22)
		valuefSneakCombatOffsetZ = v
	elseif(type == 23)
		valuefSneakMagicOffsetX = v
	elseif(type == 24)
		valuefSneakMagicOffsetZ = v
	elseif(type == 25)
		valuefSneakBowOffsetX = v
	elseif(type == 26)
		valuefSneakBowOffsetZ = v
	elseif(type == 27)
		valuefCamVelocityStrengthY = v
	elseif(type == 28)
		valuefCamVelocityStrengthZ = v
	elseif(type == 29)
		keyiShoulderSwitch = (v as int)
		ShoulderSwitchKeyRegister((v as int))
	elseif(type == 30)
		valueiShoulder = (v as int)
	endif
endEvent

event OnSyncDefault(int type, float v)
	if(type == 0)
		defaultiTickRate = v
	elseif(type == 1)
		defaultfCamSmooth = v
	elseif(type == 2)
		defaultfIdleOffsetX = v
	elseif(type == 3)
		defaultfIdleOffsetZ = v
	elseif(type == 4)
		defaultfCombatOffsetX = v
	elseif(type == 5)
		defaultfCombatOffsetZ = v
	elseif(type == 6)
		defaultfMagicOffsetX = v
	elseif(type == 7)
		defaultfMagicOffsetZ = v
	elseif(type == 8)
		defaultfBowOffsetX = v
	elseif(type == 9)
		defaultfBowOffsetZ = v
	elseif(type == 10)
		defaultfHorseIdleOffsetX = v
	elseif(type == 11)
		defaultfHorseIdleOffsetZ = v
	elseif(type == 12)
		defaultfHorseCombatOffsetX = v
	elseif(type == 13)
		defaultfHorseCombatOffsetZ = v
	elseif(type == 14)
		defaultfHorseMagicOffsetX = v
	elseif(type == 15)
		defaultfHorseMagicOffsetZ = v
	elseif(type == 16)
		defaultfHorseBowOffsetX = v
	elseif(type == 17)
		defaultfHorseBowOffsetZ = v
	elseif(type == 18)
		defaultfCamVelocityStrengthX = v
	elseif(type == 19)
		defaultfSneakIdleOffsetX = v
	elseif(type == 20)
		defaultfSneakIdleOffsetZ = v
	elseif(type == 21)
		defaultfSneakCombatOffsetX = v
	elseif(type == 22)
		defaultfSneakCombatOffsetZ = v
	elseif(type == 23)
		defaultfSneakMagicOffsetX = v
	elseif(type == 24)
		defaultfSneakMagicOffsetZ = v
	elseif(type == 25)
		defaultfSneakBowOffsetX = v
	elseif(type == 26)
		defaultfSneakBowOffsetZ = v
	elseif(type == 27)
		defaultfCamVelocityStrengthY = v
	elseif(type == 28)
		defaultfCamVelocityStrengthZ = v
	elseif(type == 29)
		defaultiShoulderSwitch = (v as int)
	elseif(type == 30)
		defaultiShoulder = (v as int)
	endif
endEvent

; @implements SKI_ConfigBase
event OnPageReset(string a_page)
	UnloadCustomContent()
	if(a_page == "$BINGLE_NGC_PAGE_SETTINGS")
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddHeaderOption("$BINGLE_NGC_SETTINGS_CAMERAMOVEMENT")
		sliderfCamSmoothOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_CAMSMOOTH", valuefCamSmooth, "{1}")
		sliderfCamVelocityStrengthXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_CAMVELSTRENGTHX", valuefCamVelocityStrengthX, "{1}")
		sliderfCamVelocityStrengthYOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_CAMVELSTRENGTHY", valuefCamVelocityStrengthY, "{1}")
		sliderfCamVelocityStrengthZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_CAMVELSTRENGTHZ", valuefCamVelocityStrengthZ, "{1}")
		keyiShoulderSwitchOID_K = AddKeyMapOption("$BINGLE_NGC_SETTINGS_SHOULDERSWITCH", keyiShoulderSwitch)
		AddEmptyOption()
		
		AddHeaderOption("$BINGLE_NGC_SETTINGS_STANDINGOFFSET")
		sliderfIdleOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_IDLEOFFSETX", valuefIdleOffsetX, "{1}")
		sliderfIdleOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_IDLEOFFSETZ", valuefIdleOffsetZ, "{1}")
		sliderfCombatOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_COMBATOFFSETX", valuefCombatOffsetX, "{1}")
		sliderfCombatOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_COMBATOFFSETZ", valuefCombatOffsetZ, "{1}")
		sliderfMagicOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_MAGICOFFSETX", valuefMagicOffsetX, "{1}")
		sliderfMagicOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_MAGICOFFSETZ", valuefMagicOffsetZ, "{1}")
		sliderfBowOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_BOWOFFSETX", valuefBowOffsetX, "{1}")
		sliderfBowOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_BOWOFFSETZ", valuefBowOffsetZ, "{1}")
	
		SetCursorPosition(1)
		AddHeaderOption("$BINGLE_NGC_SETTINGS_SNEAKOFFSET")
		sliderfSneakIdleOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_IDLEOFFSETX", valuefSneakIdleOffsetX, "{1}")
		sliderfSneakIdleOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_IDLEOFFSETZ", valuefSneakIdleOffsetZ, "{1}")
		sliderfSneakCombatOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_COMBATOFFSETX", valuefSneakCombatOffsetX, "{1}")
		sliderfSneakCombatOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_COMBATOFFSETZ", valuefSneakCombatOffsetZ, "{1}")
		sliderfSneakMagicOffsetX_S = AddSliderOption("$BINGLE_NGC_SETTINGS_MAGICOFFSETX", valuefSneakMagicOffsetX, "{1}")
		sliderfSneakMagicOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_MAGICOFFSETZ", valuefSneakMagicOffsetZ, "{1}")
		sliderfSneakBowOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_BOWOFFSETX", valuefSneakBowOffsetX, "{1}")
		sliderfSneakBowOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_BOWOFFSETZ", valuefSneakBowOffsetZ, "{1}")
		AddEmptyOption()
		
		AddHeaderOption("$BINGLE_NGC_SETTINGS_HORSEOFFSET")
		sliderfHorseIdleOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_IDLEOFFSETX", valuefHorseIdleOffsetX, "{1}")
		sliderfHorseIdleOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_IDLEOFFSETZ", valuefHorseIdleOffsetZ, "{1}")
		sliderfHorseCombatOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_COMBATOFFSETX", valuefHorseCombatOffsetX, "{1}")
		sliderfHorseCombatOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_COMBATOFFSETZ", valuefHorseCombatOffsetZ, "{1}")
		;sliderfHorseMagicOffsetX_S = AddSliderOption("$BINGLE_NGC_SETTINGS_MAGICOFFSETX", valuefHorseMagicOffsetX, "{1}")
		;sliderfHorseMagicOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_MAGICOFFSETZ", valuefHorseMagicOffsetZ, "{1}")
		sliderfHorseBowOffsetXOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_BOWOFFSETX", valuefHorseBowOffsetX, "{1}")
		sliderfHorseBowOffsetZOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_BOWOFFSETZ", valuefHorseBowOffsetZ, "{1}")
	elseif (a_page == "$BINGLE_NGC_PAGE_EXPERIMENTAL")
		slideriTickRateOID_S = AddSliderOption("$BINGLE_NGC_SETTINGS_TICKRATE", valueiTickRate, "{0}")
	endif
endEvent

event OnOptionKeyMapChange(int option, int keyCode, string conflictControl, string conflictName)
	if (option == keyiShoulderSwitchOID_K)
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
			keyiShoulderSwitch = keyCode
			SetKeymapOptionValue(keyiShoulderSwitchOID_K, keyCode)
			ShoulderSwitchKeyRegister(keyCode)
			UpdateSaveConfig(29, keyCode)
		endIf
	endIf
endEvent

string function GetCustomControl(int keyCode)
	if (keyCode == keyiShoulderSwitch)
		return "$BINGLE_NGC_KEY_SHOULDERSWITCH"
	else
		return ""
	endif
endFunction

event OnKeyDown(int keyCode)
	if(keyCode == keyiShoulderSwitch)
		valueiShoulder *= -1
		UpdateSaveConfig(30, valueiShoulder)
	endif
endEvent

event OnOptionHighlight(int option)
endEvent

event OnOptionSelect(int option)
endEvent

event OnOptionSliderOpen(int option)
	if(option == slideriTickRateOID_S)
		SetSliderDialogInterval(1)
		SetSliderDialogRange(60, 300)
		SetSliderDialogStartValue(valueiTickRate)
		SetSliderDialogDefaultValue(defaultiTickRate)
		
	elseif(option == sliderfCamSmoothOID_S)
		SetSliderDialogInterval(0.1)
		SetSliderDialogRange(1, 15)
		SetSliderDialogStartValue(valuefCamSmooth)
		SetSliderDialogDefaultValue(defaultfCamSmooth)
		
	elseif(option == sliderfCamVelocityStrengthXOID_S)
		SetSliderDialogInterval(0.1)
		SetSliderDialogRange(0, 30)
		SetSliderDialogStartValue(valuefCamVelocityStrengthX)
		SetSliderDialogDefaultValue(defaultfCamVelocityStrengthX)
		
	elseif(option == sliderfCamVelocityStrengthYOID_S)
		SetSliderDialogInterval(0.1)
		SetSliderDialogRange(0, 30)
		SetSliderDialogStartValue(valuefCamVelocityStrengthY)
		SetSliderDialogDefaultValue(defaultfCamVelocityStrengthY)
		
	elseif(option == sliderfCamVelocityStrengthZOID_S)
		SetSliderDialogInterval(0.1)
		SetSliderDialogRange(0, 30)
		SetSliderDialogStartValue(valuefCamVelocityStrengthZ)
		SetSliderDialogDefaultValue(defaultfCamVelocityStrengthZ)
		
	elseif(option == sliderfIdleOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefIdleOffsetX)
		SetSliderDialogDefaultValue(defaultfIdleOffsetX)
		
	elseif(option == sliderfIdleOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefIdleOffsetZ)
		SetSliderDialogDefaultValue(defaultfIdleOffsetZ)
		
	elseif(option == sliderfCombatOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefCombatOffsetX)
		SetSliderDialogDefaultValue(defaultfCombatOffsetX)
		
	elseif(option == sliderfCombatOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefCombatOffsetZ)
		SetSliderDialogDefaultValue(defaultfCombatOffsetZ)
		
	elseif(option == sliderfMagicOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefMagicOffsetX)
		SetSliderDialogDefaultValue(defaultfMagicOffsetX)
		
	elseif(option == sliderfMagicOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefMagicOffsetZ)
		SetSliderDialogDefaultValue(defaultfMagicOffsetZ)
		
	elseif(option == sliderfBowOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefBowOffsetX)
		SetSliderDialogDefaultValue(defaultfBowOffsetX)
		
	elseif(option == sliderfBowOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefBowOffsetZ)
		SetSliderDialogDefaultValue(defaultfBowOffsetZ)
		
	elseif(option == sliderfHorseIdleOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseIdleOffsetX)
		SetSliderDialogDefaultValue(defaultfHorseIdleOffsetX)
		
	elseif(option == sliderfHorseIdleOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseIdleOffsetZ)
		SetSliderDialogDefaultValue(defaultfHorseIdleOffsetZ)
		
	elseif(option == sliderfHorseCombatOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseCombatOffsetX)
		SetSliderDialogDefaultValue(defaultfHorseCombatOffsetX)
		
	elseif(option == sliderfHorseCombatOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseCombatOffsetZ)
		SetSliderDialogDefaultValue(defaultfHorseCombatOffsetZ)
		
	elseif(option == sliderfHorseMagicOffsetX_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseMagicOffsetX)
		SetSliderDialogDefaultValue(defaultfHorseMagicOffsetX)
		
	elseif(option == sliderfHorseMagicOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseMagicOffsetZ)
		SetSliderDialogDefaultValue(defaultfHorseMagicOffsetZ)
		
	elseif(option == sliderfHorseBowOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseBowOffsetX)
		SetSliderDialogDefaultValue(defaultfHorseBowOffsetX)
		
	elseif(option == sliderfHorseBowOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefHorseBowOffsetZ)
		SetSliderDialogDefaultValue(defaultfHorseBowOffsetZ)
		
	elseif(option == sliderfSneakIdleOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakIdleOffsetX)
		SetSliderDialogDefaultValue(defaultfSneakIdleOffsetX)
		
	elseif(option == sliderfSneakIdleOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakIdleOffsetZ)
		SetSliderDialogDefaultValue(defaultfSneakIdleOffsetZ)
		
	elseif(option == sliderfSneakCombatOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakCombatOffsetX)
		SetSliderDialogDefaultValue(defaultfSneakCombatOffsetX)
		
	elseif(option == sliderfSneakCombatOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakCombatOffsetZ)
		SetSliderDialogDefaultValue(defaultfSneakCombatOffsetZ)
		
	elseif(option == sliderfSneakMagicOffsetX_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakMagicOffsetX)
		SetSliderDialogDefaultValue(defaultfSneakMagicOffsetX)
		
	elseif(option == sliderfSneakMagicOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakMagicOffsetZ)
		SetSliderDialogDefaultValue(defaultfSneakMagicOffsetZ)
		
	elseif(option == sliderfSneakBowOffsetXOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakBowOffsetX)
		SetSliderDialogDefaultValue(defaultfSneakBowOffsetX)
		
	elseif(option == sliderfSneakBowOffsetZOID_S)
		SetSliderDialogInterval(0.5)
		SetSliderDialogRange(0, 100)
		SetSliderDialogStartValue(valuefSneakBowOffsetZ)
		SetSliderDialogDefaultValue(defaultfSneakBowOffsetZ)
		
	endif
endEvent

event OnOptionSliderAccept(int option, float value)
	if(option == slideriTickRateOID_S)
		valueiTickRate = value
		SetSliderOptionValue(slideriTickRateOID_S, valueiTickRate)
		UpdateSaveConfig(0, valueiTickRate)
		
	elseif(option == sliderfCamSmoothOID_S)
		valuefCamSmooth = value
		SetSliderOptionValue(sliderfCamSmoothOID_S, valuefCamSmooth, "{1}")
		UpdateSaveConfig(1, valuefCamSmooth)
		
	elseif(option == sliderfCamVelocityStrengthXOID_S)
		valuefCamVelocityStrengthX = value
		SetSliderOptionValue(sliderfCamVelocityStrengthXOID_S, valuefCamVelocityStrengthX, "{1}")
		UpdateSaveConfig(18, valuefCamVelocityStrengthX)
		
	elseif(option == sliderfCamVelocityStrengthYOID_S)
		valuefCamVelocityStrengthY = value
		SetSliderOptionValue(sliderfCamVelocityStrengthYOID_S, valuefCamVelocityStrengthY, "{1}")
		UpdateSaveConfig(27, valuefCamVelocityStrengthY)
		
	elseif(option == sliderfCamVelocityStrengthZOID_S)
		valuefCamVelocityStrengthZ = value
		SetSliderOptionValue(sliderfCamVelocityStrengthZOID_S, valuefCamVelocityStrengthZ, "{1}")
		UpdateSaveConfig(28, valuefCamVelocityStrengthZ)
		
	elseif(option == sliderfIdleOffsetXOID_S)
		valuefIdleOffsetX = value
		SetSliderOptionValue(sliderfIdleOffsetXOID_S, valuefIdleOffsetX, "{1}")
		UpdateSaveConfig(2, valuefIdleOffsetX)
		
	elseif(option == sliderfIdleOffsetZOID_S)
		valuefIdleOffsetZ = value
		SetSliderOptionValue(sliderfIdleOffsetZOID_S, valuefIdleOffsetZ, "{1}")
		UpdateSaveConfig(3, valuefIdleOffsetZ)
		
	elseif(option == sliderfCombatOffsetXOID_S)
		valuefCombatOffsetX = value
		SetSliderOptionValue(sliderfCombatOffsetXOID_S, valuefCombatOffsetX, "{1}")
		UpdateSaveConfig(4, valuefCombatOffsetX)
		
	elseif(option == sliderfCombatOffsetZOID_S)
		valuefCombatOffsetZ = value
		SetSliderOptionValue(sliderfCombatOffsetZOID_S, valuefCombatOffsetZ, "{1}")
		UpdateSaveConfig(5, valuefCombatOffsetZ)
		
	elseif(option == sliderfMagicOffsetXOID_S)
		valuefMagicOffsetX = value
		SetSliderOptionValue(sliderfMagicOffsetXOID_S, valuefMagicOffsetX, "{1}")
		UpdateSaveConfig(6, valuefMagicOffsetX)
		
	elseif(option == sliderfMagicOffsetZOID_S)
		valuefMagicOffsetZ = value
		SetSliderOptionValue(sliderfMagicOffsetZOID_S, valuefMagicOffsetZ, "{1}")
		UpdateSaveConfig(7, valuefMagicOffsetZ)
		
	elseif(option == sliderfBowOffsetXOID_S)
		valuefBowOffsetX = value
		SetSliderOptionValue(sliderfBowOffsetXOID_S, valuefBowOffsetX, "{1}")
		UpdateSaveConfig(8, valuefBowOffsetX)
		
	elseif(option == sliderfBowOffsetZOID_S)
		valuefBowOffsetZ = value
		SetSliderOptionValue(sliderfBowOffsetZOID_S, valuefBowOffsetZ, "{1}")
		UpdateSaveConfig(9, valuefBowOffsetZ)
		
	elseif(option == sliderfHorseIdleOffsetXOID_S)
		valuefHorseIdleOffsetX = value
		SetSliderOptionValue(sliderfHorseIdleOffsetXOID_S, valuefHorseIdleOffsetX, "{1}")
		UpdateSaveConfig(10, valuefHorseIdleOffsetX)
		
	elseif(option == sliderfHorseIdleOffsetZOID_S)
		valuefHorseIdleOffsetZ = value
		SetSliderOptionValue(sliderfHorseIdleOffsetZOID_S, valuefHorseIdleOffsetZ, "{1}")
		UpdateSaveConfig(11, valuefHorseIdleOffsetZ)
		
	elseif(option == sliderfHorseCombatOffsetXOID_S)
		valuefHorseCombatOffsetX = value
		SetSliderOptionValue(sliderfHorseCombatOffsetXOID_S, valuefHorseCombatOffsetX, "{1}")
		UpdateSaveConfig(12, valuefHorseCombatOffsetX)
		
	elseif(option == sliderfHorseCombatOffsetZOID_S)
		valuefHorseCombatOffsetZ = value
		SetSliderOptionValue(sliderfHorseCombatOffsetZOID_S, valuefHorseCombatOffsetZ, "{1}")
		UpdateSaveConfig(13, valuefHorseCombatOffsetZ)
		
	elseif(option == sliderfHorseMagicOffsetX_S)
		valuefHorseMagicOffsetX = value
		SetSliderOptionValue(sliderfHorseMagicOffsetX_S, valuefHorseMagicOffsetX, "{1}")
		UpdateSaveConfig(14, valuefHorseMagicOffsetX)
		
	elseif(option == sliderfHorseMagicOffsetZOID_S)
		valuefHorseMagicOffsetZ = value
		SetSliderOptionValue(sliderfHorseMagicOffsetZOID_S, valuefHorseMagicOffsetZ, "{1}")
		UpdateSaveConfig(15, valuefHorseMagicOffsetZ)
		
	elseif(option == sliderfHorseBowOffsetXOID_S)
		valuefHorseBowOffsetX = value
		SetSliderOptionValue(sliderfHorseBowOffsetXOID_S, valuefHorseBowOffsetX, "{1}")
		UpdateSaveConfig(16, valuefHorseBowOffsetX)
		
	elseif(option == sliderfHorseBowOffsetZOID_S)
		valuefHorseBowOffsetZ = value
		SetSliderOptionValue(sliderfHorseBowOffsetZOID_S, valuefHorseBowOffsetZ, "{1}")
		UpdateSaveConfig(17, valuefHorseBowOffsetZ)
		
	elseif(option == sliderfSneakIdleOffsetXOID_S)
		valuefSneakIdleOffsetX = value
		SetSliderOptionValue(sliderfSneakIdleOffsetXOID_S, valuefSneakIdleOffsetX, "{1}")
		UpdateSaveConfig(19, valuefSneakIdleOffsetX)
		
	elseif(option == sliderfSneakIdleOffsetZOID_S)
		valuefSneakIdleOffsetZ = value
		SetSliderOptionValue(sliderfSneakIdleOffsetZOID_S, valuefSneakIdleOffsetZ, "{1}")
		UpdateSaveConfig(20, valuefSneakIdleOffsetZ)
		
	elseif(option == sliderfSneakCombatOffsetXOID_S)
		valuefSneakCombatOffsetX = value
		SetSliderOptionValue(sliderfSneakCombatOffsetXOID_S, valuefSneakCombatOffsetX, "{1}")
		UpdateSaveConfig(21, valuefSneakCombatOffsetX)
		
	elseif(option == sliderfSneakCombatOffsetZOID_S)
		valuefSneakCombatOffsetZ = value
		SetSliderOptionValue(sliderfSneakCombatOffsetZOID_S, valuefSneakCombatOffsetZ, "{1}")
		UpdateSaveConfig(22, valuefSneakCombatOffsetZ)
		
	elseif(option == sliderfSneakMagicOffsetX_S)
		valuefSneakMagicOffsetX = value
		SetSliderOptionValue(sliderfSneakMagicOffsetX_S, valuefSneakMagicOffsetX, "{1}")
		UpdateSaveConfig(23, valuefSneakMagicOffsetX)
		
	elseif(option == sliderfSneakMagicOffsetZOID_S)
		valuefSneakMagicOffsetZ = value
		SetSliderOptionValue(sliderfSneakMagicOffsetZOID_S, valuefSneakMagicOffsetZ, "{1}")
		UpdateSaveConfig(24, valuefSneakMagicOffsetZ)
		
	elseif(option == sliderfSneakBowOffsetXOID_S)
		valuefSneakBowOffsetX = value
		SetSliderOptionValue(sliderfSneakBowOffsetXOID_S, valuefSneakBowOffsetX, "{1}")
		UpdateSaveConfig(25, valuefSneakBowOffsetX)
		
	elseif(option == sliderfSneakBowOffsetZOID_S)
		valuefSneakBowOffsetZ = value
		SetSliderOptionValue(sliderfSneakBowOffsetZOID_S, valuefSneakBowOffsetZ, "{1}")
		UpdateSaveConfig(26, valuefSneakBowOffsetZ)
		
	endif
endEvent