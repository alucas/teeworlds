#include "hook.h"
#include "hook_retracted.h"

CHookRetracted::CHookRetracted()
{
	m_HookState = HOOK_RETRACTED;
}

CHookRetracted::~CHookRetracted()
{
}

CHookRetracted*
CHookRetracted::getInstance()
{
	static CHookRetracted singleton;
	return &singleton;
}

void CHookRetracted::Execute(CHook*hook, bool UseInput)
{
	hook->m_pOwner->m_TriggeredEvents |= COREEVENT_HOOK_RETRACT;
}
