#ifdef CONF_UNITTESTING

#ifndef MOCK_OBJS_H
#define MOCK_OBJS_H

#include "game/collision.h"
#include "game/gamecore.h"

class MockCollision: public CCollision
{
public:
	MockCollision(){}

	virtual bool CheckPoint(float x, float y);
	virtual int IntersectLine(vec2 Pos0, vec2 Pos1, vec2 *pOutCollision, vec2 *pOutBeforeCollision);
};

class MockWorldCore : public CWorldCore
{
public:
	MockWorldCore(){};  
  
	virtual float GetHookLength();
};

#endif /* MOCK_OBJS_H */
#endif /* CONF_UNITTESTING */
