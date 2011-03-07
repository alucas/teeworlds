#ifndef MOCK_COLLISION_H
#define MOCK_COLLISION_H

#include <game/collision.h>

class MockCollision: public CCollision
{
 public:
  MockCollision(){
  };

  virtual bool CheckPoint(float x, float y) { 
    return true; 
  }

  virtual int IntersectLine(vec2 Pos0, vec2 Pos1, vec2 *pOutCollision, vec2 *pOutBeforeCollision){
    return 0;
  };


};
 
#endif
