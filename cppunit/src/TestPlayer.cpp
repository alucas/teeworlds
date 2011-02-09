//--- Hello, World! for CppUnit

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
  CPPUNIT_TEST(PlayerTick);
  //CPPUNIT_TEST(testPlayer);

  CPPUNIT_TEST_SUITE_END();

  MockGameWorld *gw;
  CEntity *entity;

public:
  void setUp(void) {
    gw = new MockGameWorld();
    gw->SetGameServer(new MockGameServer());
    entity = new CEntity(gw,0);


  }
  void tearDown(void) {
    /*    delete gw;
	  delete entity;*/
  } 

protected:
  

  void PlayerTick(void) {
    entity->Tick();

    //int GetCID() const { return m_ClientID; };



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
