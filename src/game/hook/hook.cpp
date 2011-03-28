#include "state.h"
#include "hook.h"
#include "hook_idle.h"
#include "hook_flying.h"
#include "hook_grabbed.h"
#include "hook_retracted.h"

CHook::CHook(CCharacterCore *character)
{
	m_pOwner = character;
	m_pCurrentState = CHookIdle::getInstance();
	m_HookState = HOOK_IDLE;
	m_HookPos = vec2(0,0);
	m_HookDir = vec2(0,0);
	m_TargetDirection = vec2(0,0);

	m_HookTick = 0;
}

void CHook::Reset()
{
	m_HookPos = m_pOwner->m_Pos;
	m_pOwner->m_HookedPlayer = -1;

	m_HookDir         = vec2(0,0);
	m_TargetDirection = vec2(0,0);

	m_HookTick = 0;
	m_pCurrentState = CHookIdle::getInstance();
	m_HookState = HOOK_IDLE;
}

void CHook::HookTick(bool useInput)
{
	m_TargetDirection = normalize(vec2(m_pOwner->m_Input.m_TargetX, m_pOwner->m_Input.m_TargetY));

	if(useInput)
	{
		if(!(m_pOwner->m_Input.m_Hook))
		{
			this->Reset();
		}
	}

	//All the code below will be replaced by :
	//m_pCurrentState->Execute(this,useInput);

	if(m_HookState == HOOK_IDLE)
	{
		m_pCurrentState->Execute(this,useInput);
	}
	else if(m_HookState == HOOK_RETRACTED)
	{
		this->m_pCurrentState = CHookRetracted::getInstance();
		this->m_pCurrentState->Execute(this, true);
	}
	else if(m_HookState == HOOK_FLYING)
	{
		m_pCurrentState = CHookFlying::getInstance();
		m_pCurrentState->Execute(this, true);
	}
	else if(m_HookState == HOOK_GRABBED)
	{
		m_pCurrentState = CHookGrabbed::getInstance();
		m_pCurrentState->Execute(this,true);
	}
}
