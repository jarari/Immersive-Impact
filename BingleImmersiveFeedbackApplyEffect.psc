Scriptname BingleImmersiveFeedbackApplyEffect extends activemagiceffect  

Spell property BingleHitFeedbackEffectSPEL auto

Event OnEffectStart(Actor akTarget, Actor akCaster)
	akTarget.AddSpell(BingleHitFeedbackEffectSPEL)
EndEvent