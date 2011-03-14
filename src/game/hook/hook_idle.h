#ifndef HOOK_IDLE_H
#define HOOK_IDLE_H

#include "state.h"
#include "hook_flying.h"
#include <iostream>

class CHookIdle: public CState{

 public:
  void Execute(CHook*hook, bool UseInput);
    ~CHookIdle();
    static CHookIdle* getInstance();
    virtual void printme(CHook * hook){ std::cout <<"Je suis Idle" << std::endl;       
      std::cout << hook->m_HookState << std::endl; };


 private:
    CHookIdle();
    CHookIdle(const CHookIdle &);
    CHookIdle& operator=(const CHookIdle &);
};

#endif
