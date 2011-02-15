#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>


#include "mockobjs.h"


class Test : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(testSnap);
  CPPUNIT_TEST_SUITE_END();

  MockGameWorld *gw;
  //CEntity *entity;
  MockGameServer *mgs;
  MockServer *ms;

public:
  void setUp(void) {
    
    ms = new MockServer();
    gw = new MockGameWorld(ms);
    mgs = new MockGameServer();
    gw->SetGameServer(mgs);
    
  }
  void tearDown(void) {
    //    delete gw->GameServer();

    //delete mgs; 
    //commente car cause un segfault dans le destructeur
    // de la classe mere
    delete ms;
    delete gw;
  } 

protected:
  void testSnap(void) {
    int PosX = 20;
    int PosY = 10;
    int team = 1;

    CFlag *flag = new CFlag(gw,1);
    flag->m_Pos = vec2(PosX,PosY);
    mgs->m_World.InsertEntity(flag);

    CEntity * entity = mgs->m_World.FindFirst(CGameWorld::ENTTYPE_FLAG);
    CPPUNIT_ASSERT(entity);
    CServer::NewClientCallback(0,ms);
    ms->setIngame(0);

    ms->m_SnapshotBuilder.Init();

    mgs->m_World.Snap(0);

    CNetObj_Flag  * netflag = (CNetObj_Flag *)ms->m_SnapshotBuilder.GetItem(0)->Data();
    CPPUNIT_ASSERT(netflag->m_X == PosX 
		   && netflag->m_Y == PosY 
		   && netflag->m_Team == 1 
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
