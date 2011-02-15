#ifndef MOCK_WORLDCORE_H
#define MOCK_WORLDCORE_H

#include <iostream>
#include "gamecore.h"

class MockWorldCore : public CWorldCore
{
 public:
  MockWorldCore(){ std::cout<<" constructeur mock world core " << std::endl; };  
};

#endif
