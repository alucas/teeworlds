#ifdef CONF_UNITTESTING

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "mockobjs.h"
#include "hookTest.h"

class Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Test);
	CPPUNIT_TEST(testSnap);
	CPPUNIT_TEST_SUITE_END();

	MockGameWorld *m_pMockGameWorld;
	MockGameServer *m_pMockGameServer;
	MockServer *m_pMockServer;
    
public:
	void setUp(void)
	{
		m_pMockServer = new MockServer();
		m_pMockGameWorld = new MockGameWorld(m_pMockServer);
		m_pMockGameServer = new MockGameServer(m_pMockServer);

		m_pMockGameServer->m_pController = new MockController(m_pMockGameServer);
		m_pMockGameServer->m_World.SetGameServer(m_pMockGameServer);
		m_pMockGameWorld->SetGameServer(m_pMockGameServer);
	}

	void tearDown(void) 
	{
		//delete m_pMockGameServer;
		//commented because causes a segfault in the mother class
		delete m_pMockServer;
		delete m_pMockGameWorld;
	} 

protected:

	void testSnap(void)
	{
		int PosX = 20;
		int PosY = 10;
		int team = 1;

		CFlag *flag = new CFlag(m_pMockGameWorld,1);
		flag->m_Pos = vec2(PosX,PosY);
		m_pMockGameServer->m_World.InsertEntity(flag);

		CEntity * entity = m_pMockGameServer->m_World.FindFirst(CGameWorld::ENTTYPE_FLAG);
		CPPUNIT_ASSERT(entity);

		CServer::NewClientCallback(0,m_pMockServer);
		m_pMockServer->setIngame(0);

		m_pMockServer->m_SnapshotBuilder.Init();
		m_pMockGameServer->m_World.Snap(0);

		CNetObj_Flag  * netflag = (CNetObj_Flag *)m_pMockServer->m_SnapshotBuilder.GetItem(0)->Data();
		CPPUNIT_ASSERT(netflag->m_X == PosX
			       && netflag->m_Y == PosY
			       && netflag->m_Team == team
			       && netflag->m_CarriedBy);

		delete flag;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main( int ac, char **av )
{
	//--- Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	//--- Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );        

	//--- Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener( &progress );      

	//--- Add the top suite to the test runner
	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run( controller );

	return result.wasSuccessful() ? 0 : 1;
}

#endif /* CONF_UNITTESTING */
