#include "hook.h"
#include "hook_flying.h"
#include "hook_grabbed.h"
#include "hook_retracted.h"

CHookFlying::CHookFlying(){
	m_HookState = HOOK_FLYING;
}

CHookFlying::~CHookFlying()
{
}

CHookFlying*
CHookFlying::getInstance()
{
	static CHookFlying singleton;
	return &singleton;
}

void CHookFlying::Execute(CHook*hook, bool UseInput)
{
	vec2 NewPos = (hook->m_HookPos) +  (hook->m_HookDir) * (hook->m_pOwner->m_pWorld->m_Tuning.m_HookFireSpeed);

	if(distance(hook->m_pOwner->m_Pos, NewPos) > hook->m_pOwner->m_pWorld->m_Tuning.m_HookLength)
	{
		hook->m_pCurrentState = CHookRetracted::getInstance();

		//Mock object technique used for cppunit. Please keep it
		float hookLength = hook->m_pOwner->m_pWorld->m_Tuning.m_HookLength;
		NewPos = hook->m_pOwner->m_Pos + normalize(NewPos- hook->m_pOwner->m_Pos) * hookLength;
	}

	// make sure that the hook doesn't go though the ground
	bool GoingToHitGround = false;
	bool GoingToRetract = false;

	int Hit = hook->m_pOwner->m_pCollision->IntersectLine(hook->m_HookPos, NewPos, &NewPos, 0);
	if(Hit)
	{
		if(Hit&CCollision::COLFLAG_NOHOOK)
			GoingToRetract = true;
		else
			GoingToHitGround = true;
	}

	// Check against other players first
	if(hook->m_pOwner->m_pWorld && hook->m_pOwner->m_pWorld->m_Tuning.m_PlayerHooking)
	{
		float Dist = 0.0f;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			CCharacterCore *p = hook->m_pOwner->m_pWorld->m_apCharacters[i];
			if(!p || p == hook->m_pOwner)
				continue;

			vec2 ClosestPoint = closest_point_on_line(hook->m_HookPos, NewPos, p->m_Pos);
			if(distance(p->m_Pos, ClosestPoint) < hook->m_pOwner->m_PhysSize+2.0f)
			{
				if (hook->m_pOwner->m_HookedPlayer == -1 || distance(hook->m_HookPos, p->m_Pos) < Dist)
				{
					hook->m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_PLAYER;
					hook->m_pCurrentState = CHookGrabbed::getInstance();
					hook->m_pOwner->m_HookedPlayer = i;
					Dist = distance(hook->m_HookPos, p->m_Pos);
				}
			}
		}
	}

	if(m_HookState == hook->m_pCurrentState->getState())
	{
		if(GoingToHitGround)
		{
			hook->m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_GROUND;
			hook->m_pCurrentState = CHookGrabbed::getInstance();

		}
		else if(GoingToRetract)
		{
			hook->m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_HIT_NOHOOK;
			hook->m_pCurrentState = CHookRetracted::getInstance();
		}
		hook->m_HookPos = NewPos;
	}
}
