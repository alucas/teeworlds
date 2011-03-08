#include "hook.h"
#include "state.h"

CHook::CHook(CCharacterCore *character) {
  owner = character;
  //m_pCurrentState = CHookIdle::getInstance();
    
}

void CHook::Reset()
{
	m_HookPos = vec2(0,0);
	m_HookDir = vec2(0,0);
	m_HookTick = 0;
	m_HookState = HOOK_IDLE;
}

void CHook::HookTick(bool useInput)
{

	vec2 TargetDirection = normalize(vec2(owner->m_Input.m_TargetX, owner->m_Input.m_TargetY));

	if(useInput)
	{
		if(owner->m_Input.m_Hook)
		{
			if(m_HookState == HOOK_IDLE)
			{
				m_HookState = HOOK_FLYING;
				m_HookPos = owner->m_Pos + TargetDirection* owner->PhysSize*1.5f;
				m_HookDir = TargetDirection;
				owner->m_HookedPlayer = -1;
				m_HookTick = 0;
				owner->m_TriggeredEvents |= COREEVENT_HOOK_LAUNCH;
			}		
		}
		else
		{
			owner->m_HookedPlayer = -1;
			m_HookState = HOOK_IDLE;
			m_HookPos = owner->m_Pos;			
		}	
	}

	if(m_HookState == HOOK_IDLE)
	{
		owner->m_HookedPlayer = -1;
		m_HookState = HOOK_IDLE;
		m_HookPos = owner->m_Pos;
	}
	else if(m_HookState >= HOOK_RETRACT_START && m_HookState < HOOK_RETRACT_END)
	{
		m_HookState++;
	}
	else if(m_HookState == HOOK_RETRACT_END)
	{
		m_HookState = HOOK_RETRACTED;
		owner->m_TriggeredEvents |= COREEVENT_HOOK_RETRACT;
		m_HookState = HOOK_RETRACTED;
	}
	else if(m_HookState == HOOK_FLYING)
	{
		vec2 NewPos = (m_HookPos) +  (m_HookDir) * (owner->m_pWorld->m_Tuning.m_HookFireSpeed);

		if(distance(owner->m_Pos, NewPos) > owner->m_pWorld->m_Tuning.m_HookLength)
		{
			m_HookState = HOOK_RETRACT_START;
			//Mock object technique used for cppunit. Please keep it 
			float hookLength = owner->m_pWorld->GetHookLength();
			//PLUS it clarifies this expression :)
			NewPos = owner->m_Pos + normalize(NewPos-owner->m_Pos) * hookLength;
		}
		
		// make sure that the hook doesn't go though the ground
		bool GoingToHitGround = false;
		bool GoingToRetract = false;

		int Hit = owner->m_pCollision->IntersectLine(m_HookPos, NewPos, &NewPos, 0);
		if(Hit)
		{
			if(Hit&CCollision::COLFLAG_NOHOOK)
				GoingToRetract = true;
			else
				GoingToHitGround = true;
		}

		// Check against other players first
		if(owner->m_pWorld && owner->m_pWorld->m_Tuning.m_PlayerHooking)
		{
			float Dist = 0.0f;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				CCharacterCore *p = owner->m_pWorld->m_apCharacters[i];
				if(!p || p == owner)
					continue;

				vec2 ClosestPoint = closest_point_on_line(m_HookPos, NewPos, p->m_Pos);
				if(distance(p->m_Pos, ClosestPoint) < owner->PhysSize+2.0f)
				{
					if (owner->m_HookedPlayer == -1 || distance(m_HookPos, p->m_Pos) < Dist)
					{
						owner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_PLAYER;
						m_HookState = HOOK_GRABBED;
						owner->m_HookedPlayer = i;
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
				owner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_GROUND;
				m_HookState = HOOK_GRABBED;
			}
			else if(GoingToRetract)
			{
				owner->m_TriggeredEvents |= COREEVENT_HOOK_HIT_NOHOOK;
				m_HookState = HOOK_RETRACT_START;
			}
			
			m_HookPos = NewPos;
		}
	}
	
	if(m_HookState == HOOK_GRABBED)
	{
		if(owner->m_HookedPlayer != -1)
		{
			CCharacterCore *p = owner->m_pWorld->m_apCharacters[ owner->m_HookedPlayer];
			if(p)
				m_HookPos = p->m_Pos;
			else
			{
				// release hook
				owner->m_HookedPlayer = -1;
				m_HookState = HOOK_RETRACTED;
				m_HookPos = owner->m_Pos;					
			}
			
			// keep players hooked for a max of 1.5sec
			//if(Server()->Tick() > hook_tick+(Server()->TickSpeed()*3)/2)
			//release_hooked();
		}
		
		// don't do this hook rutine when we are hook to a player
		if(owner->m_HookedPlayer == -1 && distance(m_HookPos, owner->m_Pos) > 46.0f)
		{
			vec2 HookVel = normalize(m_HookPos-owner->m_Pos)* owner->m_pWorld->m_Tuning.m_HookDragAccel;
			// the hook as more power to drag you up then down.
			// this makes it easier to get on top of an platform
			if(HookVel.y > 0)
				HookVel.y *= 0.3f;
			
			// the hook will boost it's power if the player wants to move
			// in that direction. otherwise it will dampen everything abit
			if((HookVel.x < 0 && owner->m_Direction < 0) || (HookVel.x > 0 && owner->m_Direction > 0)) 
				HookVel.x *= 0.95f;
			else
				HookVel.x *= 0.75f;
			
			vec2 NewVel = owner->m_Vel+HookVel;

			// check if we are under the legal limit for the hook
			if(length(NewVel) < owner->m_pWorld->m_Tuning.m_HookDragSpeed || length(NewVel) < length(owner->m_Vel))
				owner->m_Vel = NewVel; // no problem. apply
				
		}

		// release hook (max hook time is 1.25
		m_HookTick++;
		if(owner->m_HookedPlayer != -1 && (m_HookTick > SERVER_TICK_SPEED+SERVER_TICK_SPEED/5 || ! owner->m_pWorld->m_apCharacters[owner->m_HookedPlayer]))
		{
			owner->m_HookedPlayer = -1;
			m_HookState = HOOK_RETRACTED;
			m_HookPos = owner->m_Pos;			
		}
	}

}
