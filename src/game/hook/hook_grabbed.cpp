#include "hook.h"
#include "hook_grabbed.h"
#include "hook_retracted.h"

CHookGrabbed::CHookGrabbed()
{
	m_HookState = HOOK_GRABBED;
}

CHookGrabbed::~CHookGrabbed()
{
}

CHookGrabbed*
CHookGrabbed::getInstance()
{
	static CHookGrabbed singleton;
	return &singleton;
}

void CHookGrabbed::Execute(CHook*hook, bool UseInput)
{
	if(hook->m_pOwner->m_HookedPlayer != -1)
	{
		CCharacterCore *p = hook->m_pOwner->m_pWorld->m_apCharacters[ hook->m_pOwner->m_HookedPlayer];
		if(p)
			hook->m_HookPos = p->m_Pos;
		else
		{
			// release hook
			hook->m_pOwner->m_HookedPlayer = -1;
			hook->m_pCurrentState = CHookRetracted::getInstance();
			hook->m_HookPos = hook->m_pOwner->m_Pos;
		}
	}

	// don't do this hook rutine when we are hook to a player
	if(hook->m_pOwner->m_HookedPlayer == -1 && distance(hook->m_HookPos, hook->m_pOwner->m_Pos) > 46.0f)
	{
		vec2 HookVel = normalize(hook->m_HookPos-hook->m_pOwner->m_Pos)* hook->m_pOwner->m_pWorld->m_Tuning.m_HookDragAccel;
		// the hook as more power to drag you up then down.
		// this makes it easier to get on top of an platform
		if(HookVel.y > 0)
			HookVel.y *= 0.3f;

		// the hook will boost it's power if the player wants to move
		// in that direction. otherwise it will dampen everything abit
		if((HookVel.x < 0 && hook->m_pOwner->m_Direction < 0) || (HookVel.x > 0 && hook->m_pOwner->m_Direction > 0))
			HookVel.x *= 0.95f;
		else
			HookVel.x *= 0.75f;

		vec2 NewVel = hook->m_pOwner->m_Vel+HookVel;

		// check if we are under the legal limit for the hook
		if(length(NewVel) < hook->m_pOwner->m_pWorld->m_Tuning.m_HookDragSpeed || length(NewVel) < length(hook->m_pOwner->m_Vel))
			hook->m_pOwner->m_Vel = NewVel; // no problem. apply
	}

	// release hook (max hook time is 1.25
	hook->m_HookTick++;
	if(hook->m_pOwner->m_HookedPlayer != -1 &&
			(hook->m_HookTick > SERVER_TICK_SPEED+SERVER_TICK_SPEED/5 || ! hook->m_pOwner->m_pWorld->m_apCharacters[hook->m_pOwner->m_HookedPlayer]))
	{
		hook->m_pOwner->m_HookedPlayer = -1;
		hook->m_pCurrentState = CHookRetracted::getInstance();

		hook->m_HookPos = hook->m_pOwner->m_Pos;
	}
}
