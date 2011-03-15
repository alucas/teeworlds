#include <iostream>
#include "hook.h"
#include "hook_flying.h"
#include "hook_grabbed.h"
#include "hook_retracted.h"

CHookRetracted::CHookRetracted(){

}

CHookRetracted::~CHookRetracted(){

}

CHookRetracted* 
CHookRetracted::getInstance(){

  static CHookRetracted singleton;
  return &singleton;

} 

void
CHookRetracted::Execute(CHook*hook, bool UseInput){

  //std::cout << "c'est ca ?" << std::endl;
   if(hook->m_pCurrentState->giveMeState() != HOOK_RETRACTED)
     hook->m_pCurrentState->printme(hook);

   //hook->m_HookState = HOOK_RETRACTED;
  hook->owner->m_TriggeredEvents |= COREEVENT_HOOK_RETRACT;

}
