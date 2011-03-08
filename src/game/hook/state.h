#ifndef STATE_H
#define STATE_H

#include "hook.h"

//A state in a finite deterministic automaton.

class CState
{
public:
  virtual void Execute(CHook*)=0;
  virtual ~CState()=0;


  CState();

 private:

  //Singleton
  CState(const CState &);
  CState& operator=(const CState &);
}; 

#endif
