#include "hook.h"
#include "hook_idle.h"
#include "hook_flying.h"

CHookIdle::CHookIdle()
{
	m_HookState = HOOK_IDLE;
}

CHookIdle::~CHookIdle()
{
}

CHookIdle*
CHookIdle::getInstance()
{
	static CHookIdle singleton;
	return &singleton;
}

void CHookIdle::Execute(CHook*hook, bool UseInput)
{
	if( UseInput && hook->m_pOwner->m_Input.m_Hook)
	{
		hook->m_pCurrentState = CHookFlying::getInstance();

		hook->m_HookPos = hook->m_pOwner->m_Pos + hook->m_TargetDirection* hook->m_pOwner->m_PhysSize*1.5f;
		hook->m_HookDir = hook->m_TargetDirection;
		hook->m_pOwner->m_HookedPlayer = -1;
		hook->m_HookTick = 0;
		hook->m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_LAUNCH;
	}
	else
	{
		hook->Reset();
	}
}
