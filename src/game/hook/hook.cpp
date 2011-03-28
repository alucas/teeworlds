#include "hook.h"

void CHook::Reset()
{
	m_HookPos = vec2(0,0);
	m_HookDir = vec2(0,0);
	m_HookTick = 0;
	m_HookState = HOOK_IDLE;
}

void CHook::HookTick(bool useInput)
{
	vec2 TargetDirection = normalize(vec2(m_pOwner->m_Input.m_TargetX, m_pOwner->m_Input.m_TargetY));

	if(useInput)
	{
		if(m_pOwner->m_Input.m_Hook)
		{
			if(m_HookState == HOOK_IDLE)
			{
				m_HookState = HOOK_FLYING;
				m_HookPos = m_pOwner->m_Pos + TargetDirection* m_pOwner->m_PhysSize*1.5f;
				m_HookDir = TargetDirection;
				m_pOwner->m_HookedPlayer = -1;
				m_HookTick = 0;
				m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_LAUNCH;
			}
		}
		else
		{
			m_pOwner->m_HookedPlayer = -1;
			m_HookState = HOOK_IDLE;
			m_HookPos = m_pOwner->m_Pos;
		}
	}

	if(m_HookState == HOOK_IDLE)
	{
		m_pOwner->m_HookedPlayer = -1;
		m_HookState = HOOK_IDLE;
		m_HookPos = m_pOwner->m_Pos;
	}
	else if(m_HookState >= HOOK_RETRACT_START && m_HookState < HOOK_RETRACT_END)
	{
		m_HookState++;
	}
	else if(m_HookState == HOOK_RETRACT_END)
	{
		m_HookState = HOOK_RETRACTED;
		m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_RETRACT;
		m_HookState = HOOK_RETRACTED;
	}
	else if(m_HookState == HOOK_FLYING)
	{
		vec2 NewPos = (m_HookPos) +  (m_HookDir) * (m_pOwner->m_pWorld->m_Tuning.m_HookFireSpeed);

		if(distance(m_pOwner->m_Pos, NewPos) > m_pOwner->m_pWorld->m_Tuning.m_HookLength)
		{
			m_HookState = HOOK_RETRACT_START;
			float HookLength = m_pOwner->m_pWorld->m_Tuning.m_HookLength;
			NewPos = m_pOwner->m_Pos + normalize(NewPos-m_pOwner->m_Pos) * HookLength;
		}

		// make sure that the hook doesn't go though the ground
		bool GoingToHitGround = false;
		bool GoingToRetract = false;

		int Hit = m_pOwner->m_pCollision->IntersectLine(m_HookPos, NewPos, &NewPos, 0);
		if(Hit)
		{
			if(Hit&CCollision::COLFLAG_NOHOOK)
				GoingToRetract = true;
			else
				GoingToHitGround = true;
		}

		// Check against other players first
		if(m_pOwner->m_pWorld && m_pOwner->m_pWorld->m_Tuning.m_PlayerHooking)
		{
			float Dist = 0.0f;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				CCharacterCore *p = m_pOwner->m_pWorld->m_apCharacters[i];
				if(!p || p == m_pOwner)
					continue;

				vec2 ClosestPoint = closest_point_on_line(m_HookPos, NewPos, p->m_Pos);
				if(distance(p->m_Pos, ClosestPoint) < m_pOwner->m_PhysSize+2.0f)
				{
					if (m_pOwner->m_HookedPlayer == -1 || distance(m_HookPos, p->m_Pos) < Dist)
					{
						m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_PLAYER;
						m_HookState = HOOK_GRABBED;
						m_pOwner->m_HookedPlayer = i;
						Dist = distance(m_HookPos, p->m_Pos);
					}
				}
			}
		}

		if(m_HookState == HOOK_FLYING)
		{
			// check against ground
			if(GoingToHitGround)
			{
				m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_GROUND;
				m_HookState = HOOK_GRABBED;
			}
			else if(GoingToRetract)
			{
				m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_HIT_NOHOOK;
				m_HookState = HOOK_RETRACT_START;
			}

			m_HookPos = NewPos;
		}
	}

	if(m_HookState == HOOK_GRABBED)
	{
		if(m_pOwner->m_HookedPlayer != -1)
		{
			CCharacterCore *p = m_pOwner->m_pWorld->m_apCharacters[ m_pOwner->m_HookedPlayer];
			if(p)
				m_HookPos = p->m_Pos;
			else
			{
				// release hook
				m_pOwner->m_HookedPlayer = -1;
				m_HookState = HOOK_RETRACTED;
				m_HookPos = m_pOwner->m_Pos;
			}

			// keep players hooked for a max of 1.5sec
			//if(Server()->Tick() > hook_tick+(Server()->TickSpeed()*3)/2)
			//release_hooked();
		}

		// don't do this hook rutine when we are hook to a player
		if(m_pOwner->m_HookedPlayer == -1 && distance(m_HookPos, m_pOwner->m_Pos) > 46.0f)
		{
			vec2 HookVel = normalize(m_HookPos-m_pOwner->m_Pos)* m_pOwner->m_pWorld->m_Tuning.m_HookDragAccel;
			// the hook as more power to drag you up then down.
			// this makes it easier to get on top of an platform
			if(HookVel.y > 0)
				HookVel.y *= 0.3f;

			// the hook will boost it's power if the player wants to move
			// in that direction. otherwise it will dampen everything abit
			if((HookVel.x < 0 && m_pOwner->m_Direction < 0) || (HookVel.x > 0 && m_pOwner->m_Direction > 0))
				HookVel.x *= 0.95f;
			else
				HookVel.x *= 0.75f;

			vec2 NewVel = m_pOwner->m_Vel+HookVel;

			// check if we are under the legal limit for the hook
			if(length(NewVel) < m_pOwner->m_pWorld->m_Tuning.m_HookDragSpeed || length(NewVel) < length(m_pOwner->m_Vel))
				m_pOwner->m_Vel = NewVel; // no problem. apply
		}

		// release hook (max hook time is 1.25
		m_HookTick++;
		if(m_pOwner->m_HookedPlayer != -1 && (m_HookTick > SERVER_TICK_SPEED+SERVER_TICK_SPEED/5 || ! m_pOwner->m_pWorld->m_apCharacters[m_pOwner->m_HookedPlayer]))
		{
			m_pOwner->m_HookedPlayer = -1;
			m_HookState = HOOK_RETRACTED;
			m_HookPos = m_pOwner->m_Pos;
		}
	}
}
