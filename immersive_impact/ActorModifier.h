#pragma once

class Actor;
class ActorModifier {
public:
	static void RestrainMovement(Actor *a, bool restrain);
	static void RestrainView(Actor *a, bool restrain);
	static void ModifyAV(Actor* a, char *AVname, float v);
	static void SetCurrentAV(Actor* a, char *AVname, float v);
	static float GetAV(Actor * a, char * AVname);
	static float GetAVMax(Actor * a, char * AVname);
	static void EnableRestraint(bool yes);
};