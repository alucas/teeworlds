#include "state.h"
#include "hook.h"
#include "hook_idle.h"
#include "hook_flying.h"
#include "hook_grabbed.h"
#include "hook_retracted.h"


CHook::CHook(CCharacterCore *character) {
  owner = character;
  m_pCurrentState = CHookIdle::getInstance();
  m_HookState = 0;
  m_HookPos         = vec2(0,0);
  m_HookDir         = vec2(0,0);
  m_TargetDirection = vec2(0,0);
  
  m_HookTick = 0;
    
}

void CHook::Reset()
{
  // m_HookPos         = vec2(0,0);
  m_HookPos = owner->m_Pos;			
  owner->m_HookedPlayer = -1;

  m_HookDir         = vec2(0,0);
  m_TargetDirection = vec2(0,0);
  
  m_HookTick = 0;
  m_pCurrentState = CHookIdle::getInstance();
  m_HookState = HOOK_IDLE;
}

void CHook::HookTick(bool useInput)
{
  
	m_TargetDirection = normalize(vec2(owner->m_Input.m_TargetX, owner->m_Input.m_TargetY));

	if(useInput)
	{
	  if ( !(owner->m_Input.m_Hook)	)
	  {
	    this->Reset();
	  }	
	}


	//Il devrait y avoir juste une ligne de code, celle ci :
	// m_pCurrentState->Execute(this,useInput);


	if(m_HookState == HOOK_IDLE){

	  m_pCurrentState->Execute(this,useInput);
	  //m_pCurrentState->printme(this);

	    
	}

	else if( m_HookState == HOOK_RETRACTED){
	  //m_HookState = HOOK_RETRACTED;
	  this->m_pCurrentState =  CHookRetracted::getInstance();
	  this->m_pCurrentState->Execute(this, true);
	
	}
	
	else if(m_HookState == HOOK_FLYING)
	{
	  //A repérer pourquoi il ya un bug ici.
	  m_pCurrentState = CHookFlying::getInstance();
	  //m_pCurrentState->printme(this);
	  m_pCurrentState->Execute(this, true);

	}

	if(m_HookState == HOOK_GRABBED)
	{
	  //m_pCurrentState->printme(this);
	  m_pCurrentState = CHookGrabbed::getInstance();
	  m_pCurrentState->Execute(this,true);
	}

}
