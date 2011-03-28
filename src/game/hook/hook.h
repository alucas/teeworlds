#ifndef HOOK_H
#define HOOK_H

#include <game/gamecore.h>

class CHook{
public:
	//The char that own the current Hook.
	CCharacterCore *m_pOwner;

	vec2 m_HookPos;
	vec2 m_HookDir;
	int m_HookTick;
	int m_HookState;

	CHook(CCharacterCore *character):m_pOwner(character){ };

	void Reset();
	void HookTick(bool useInput);
	int GetHookState(){ return m_HookState;};
};

#endif
