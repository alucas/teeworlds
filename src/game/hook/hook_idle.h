#ifndef HOOK_IDLE_H
#define HOOK_IDLE_H

#include "state.h"


class CHookIdle: public CState{
  //  class CHookIdle{

 public:
  void Execute(CHook*hook, bool UseInput);
    ~CHookIdle();
    static CHookIdle* getInstance();

 private:
    CHookIdle();
    CHookIdle(const CHookIdle &);
    CHookIdle& operator=(const CHookIdle &);
};

#endif
