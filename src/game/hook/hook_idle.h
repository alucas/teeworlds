#ifndef HOOK_IDLE_H
#define HOOK_IDLE_H

#include "state.h"

class CHookIdle: public CState{

 public:
    void Execute(CHook*hook);
    ~CHookIdle();
    static CHookIdle* getInstance();

    static CHookIdle* m_pSingleton();

 private:
    CHookIdle(){};
    CHookIdle(const CHookIdle &);
    CHookIdle& operator=(const CHookIdle &);
};

#endif
