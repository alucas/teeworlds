#ifndef HOOK_FLYING_H
#define HOOK_FLYING_H

#include "state.h"
#include <iostream>
class CHookFlying: public CState{

 public:
  void Execute(CHook*hook, bool UseInput);
    ~CHookFlying();
    static CHookFlying* getInstance();
    virtual void printme(CHook* hook){ std::cout <<"Je suis flying" << std::endl;
      std::cout << hook->m_HookState << std::endl; };

 private:
    CHookFlying();
    CHookFlying(const CHookFlying &);
    CHookFlying& operator=(const CHookFlying &);
};

#endif
