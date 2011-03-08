#ifndef HOOK_H
#define HOOK_H

#include "../gamecore.h"

// hooking stuff : it's going to disapear.

class CHook{

 public:
  //The char that own the current Hook.
  CCharacterCore *owner;

  vec2 m_HookPos;
  vec2 m_HookDir;
  int m_HookTick;
  int m_HookState;


 CHook(CCharacterCore *character):owner(character){ };

  void Reset();
  void HookTick(bool useInput);
  int GetHookState(){ return m_HookState;};


};


#endif
