#ifndef HOOK_H
#define HOOK_H

#include "../gamecore.h"
#include "hook_idle.h"

class CState;

class CHook{

 public:

  //The hook has different states : flying, idle, grabbed...
  CState *m_pCurrentState;

  //The char that own the current Hook.
  CCharacterCore *owner;

  vec2 m_HookPos;
  vec2 m_HookDir;
  int m_HookTick;
  //Need to disapear;
  int m_HookState;


 CHook(CCharacterCore *character);

  void Reset();
  void HookTick(bool useInput);
  int GetHookState(){ return m_HookState;};


};


#endif
