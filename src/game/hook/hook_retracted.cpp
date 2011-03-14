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
  hook->owner->m_TriggeredEvents |= COREEVENT_HOOK_RETRACT;

}
