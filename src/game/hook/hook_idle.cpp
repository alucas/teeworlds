#include <iostream>
#include "hook.h"
#include "hook_idle.h"

CHookIdle::CHookIdle(){

}
CHookIdle::~CHookIdle(){
  std::cout <<"Chookidle NE DEVRAIT PAS ETRE APPELE" << std::endl;

}

CHookIdle* 
CHookIdle::getInstance(){

  static CHookIdle singleton;
  return &singleton;

} 

void
CHookIdle::Execute(CHook*hook, bool UseInput){

  if(UseInput)
    {
      hook->m_HookState = HOOK_FLYING;
      hook->m_HookPos = hook->owner->m_Pos + hook->m_TargetDirection* hook->owner->PhysSize*1.5f;
      hook->m_HookDir = hook->m_TargetDirection;
      hook->owner->m_HookedPlayer = -1;
      hook->m_HookTick = 0;
      hook->owner->m_TriggeredEvents |= COREEVENT_HOOK_LAUNCH;
    }
  else{

    hook->owner->m_HookedPlayer = -1;
    hook->m_HookState = HOOK_IDLE;
    hook->m_HookPos = hook->owner->m_Pos;
  }	
}
