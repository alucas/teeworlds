#include <iostream>
#include "hook.h"
#include "hook_flying.h"
#include "hook_grabbed.h"
#include "hook_retracted.h"

CHookFlying::CHookFlying(){

}

CHookFlying::~CHookFlying(){
  
}

CHookFlying* 
CHookFlying::getInstance(){

  static CHookFlying singleton;
  return &singleton;

} 

void
CHookFlying::Execute(CHook*hook, bool UseInput){

  vec2 NewPos = (hook->m_HookPos) +  (hook->m_HookDir) * (hook->owner->m_pWorld->m_Tuning.m_HookFireSpeed);

    if(hook->m_pCurrentState->giveMeState() != HOOK_FLYING)
      hook->m_pCurrentState->printme(hook);

    //hook->m_HookState = HOOK_FLYING;

  if(distance(hook->owner->m_Pos, NewPos) > hook->owner->m_pWorld->m_Tuning.m_HookLength)
    {

      hook->m_HookState = HOOK_RETRACTED;
      hook->m_pCurrentState = CHookRetracted::getInstance();

      //Mock object technique used for cppunit. Please keep it 
      float hookLength = hook->owner->m_pWorld->GetHookLength();
      //PLUS it clarifies this expression :)
      NewPos = hook->owner->m_Pos + normalize(NewPos- hook->owner->m_Pos) * hookLength;
      return;
    }
		
  // make sure that the hook doesn't go though the ground
  bool GoingToHitGround = false;
  bool GoingToRetract = false;

  int Hit = hook->owner->m_pCollision->IntersectLine(hook->m_HookPos, NewPos, &NewPos, 0);
  if(Hit)
    {
      if(Hit&CCollision::COLFLAG_NOHOOK){
	std::cout <<"going to retract passe à true" << std::endl;
	GoingToRetract = true;
      }
      else
	GoingToHitGround = true;
    }

  // Check against other players first
  if(hook->owner->m_pWorld && hook->owner->m_pWorld->m_Tuning.m_PlayerHooking)
    {
      float Dist = 0.0f;
      for(int i = 0; i < MAX_CLIENTS; i++)
	{
	  CCharacterCore *p = hook->owner->m_pWorld->m_apCharacters[i];
	  if(!p || p == hook->owner)
	    continue;

	  vec2 ClosestPoint = closest_point_on_line(hook->m_HookPos, NewPos, p->m_Pos);
	  if(distance(p->m_Pos, ClosestPoint) < hook->owner->PhysSize+2.0f)
	    {
	      if (hook->owner->m_HookedPlayer == -1 || distance(hook->m_HookPos, p->m_Pos) < Dist)
		{
		  hook->owner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_PLAYER;
		  hook->m_HookState = HOOK_GRABBED;
		  hook->m_pCurrentState = CHookGrabbed::getInstance();
		  hook->owner->m_HookedPlayer = i;
		  Dist = distance(hook->m_HookPos, p->m_Pos);
		}
	    }
	}
    }

  if(hook->m_HookState == HOOK_FLYING)
    {
      // check against ground
      if(GoingToHitGround)
	{
	  hook->owner->m_TriggeredEvents |= COREEVENT_HOOK_ATTACH_GROUND;
	  hook->m_HookState = HOOK_GRABBED;
	  hook->m_pCurrentState = CHookGrabbed::getInstance();

	}
      else if(GoingToRetract)
	{
	  hook->owner->m_TriggeredEvents |= COREEVENT_HOOK_HIT_NOHOOK;
	  std::cout << "debut hook retracted"<< std::endl;
	  hook->m_HookState = HOOK_RETRACTED;
	  hook->m_pCurrentState = CHookRetracted::getInstance();
	  std::cout << "debut hook retracted"<< std::endl;
	}
			
      hook->m_HookPos = NewPos;
    }


  
  //hook->m_pCurrentState->printme(hook);


}
