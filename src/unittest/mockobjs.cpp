#ifdef CONF_UNITTESTING

#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestRunner.h>

#include "mockobjs.h"

bool 
MockCollision::CheckPoint(float x, float y)
{ 
	return true; 
}

int 
MockCollision::IntersectLine(vec2 Pos0, vec2 Pos1, vec2 *pOutCollision, vec2 *pOutBeforeCollision)
{
	return 0;
}

float
MockWorldCore::GetHookLength()
{
	return 1; //Arbitrary value. Chosen to be low.
}

#endif /* CONF_UNITTESTING */
