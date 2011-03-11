#ifdef CONF_UNITTESTING

#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestRunner.h>

#include "mockobjs.h"

int
MockServer::Tick()
{
	return 1;
}

MockGameWorld::MockGameWorld(MockServer *mockServ):CGameWorld()
{
	m_pMockGameServer = new MockGameServer(mockServ);
	m_pServer = mockServ;
}


MockController::MockController(CGameContext *gc) : IGameController (gc)
{
}

MockGameServer::MockGameServer(MockServer *mockServ) : CGameContext()
{
	m_pMockServer = mockServ;
};

void
MockServer::setIngame(int index)
{
	m_aClients[0].m_State = CClient::STATE_INGAME;
}

int
MockController::ClampTeam(int team)
{
	return team;
}


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
