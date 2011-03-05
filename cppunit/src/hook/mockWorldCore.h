#ifndef MOCK_WORLDCORE_H
#define MOCK_WORLDCORE_H

#include <iostream>
#include "game/gamecore.h"

class MockWorldCore : public CWorldCore
{
 public:
  MockWorldCore(){};  
  
  float GetHookLength(){
    return 1; //Arbitrary value. Chosen to be low.
  }
};

#endif
