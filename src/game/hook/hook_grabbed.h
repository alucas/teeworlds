#ifndef HOOK_GRABBED_H
#define HOOK_GRABBED_H

#include "state.h"
#include <iostream>
class CHookGrabbed: public CState{

 public:
  void Execute(CHook*hook, bool UseInput);
    ~CHookGrabbed();
    static CHookGrabbed* getInstance();
    virtual void printme(CHook* hook){ std::cout <<"Je suis grabbed" << std::endl; 
         std::cout << hook->m_HookState << std::endl; };
  virtual int giveMeState(){ return HOOK_GRABBED; } 

 private:
    CHookGrabbed();
    CHookGrabbed(const CHookGrabbed &);
    CHookGrabbed& operator=(const CHookGrabbed &);
};

#endif
