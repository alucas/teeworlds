#ifndef MOCK_COLLISION_H
#define MOCK_COLLISION_H

#include <collision.h>
#include <iostream>

class MockCollision: public CCollision
{
 public:
  MockCollision(){
    std::cout << "constructeur mock collision" << std::endl;
  };

  virtual bool CheckPoint(float x, float y) { 
    std::cout << "good checkPOint" << std::endl;
    return true; 
  }


};
 
#endif
