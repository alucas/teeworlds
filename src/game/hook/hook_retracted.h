#ifndef HOOK_RETRACTED_H
#define HOOK_RETRACTED_H

#include "state.h"
#include <iostream>
class CHookRetracted: public CState{

 public:
  void Execute(CHook*hook, bool UseInput);
    ~CHookRetracted();
    static CHookRetracted* getInstance();
    virtual void printme(CHook* hook){ std::cout <<"Je suis retracted" << std::endl;
      std::cout << hook->m_HookState << std::endl; };

 private:
    CHookRetracted();
    CHookRetracted(const CHookRetracted &);
    CHookRetracted& operator=(const CHookRetracted &);
};

#endif
