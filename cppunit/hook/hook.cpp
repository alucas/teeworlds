//Tests the hook behavior.


#include <iostream>
#include <stdlib.h>
#include <math.h>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>


//Tested class, that contains CCharacter Core.
#include "gamecore.h"

#include "mockCollision.h"
#include "mockWorldCore.h"

using namespace std;


class Test : public CPPUNIT_NS::TestCase
{

  CCharacterCore charCore;
  CWorldCore *pWorld;
  MockCollision *pCollision;

  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(CharacterCoreTick);

  CPPUNIT_TEST_SUITE_END();

  /*
  MockGameWorld *gw;
  CEntity *entity;
  MockGameServer* mockGameServer;
  CPlayer *player;
  */
public:

  void setUp(void) {
    cout << "setup" << endl;
    pWorld = new MockWorldCore();
    pCollision = new MockCollision();

    charCore.Init(pWorld, pCollision);
    charCore.Reset();

  }

  void tearDown(void) {
        cout << "tear down" << endl;
  }

protected:

  void CharacterCoreTick(void) {
    cout << "start charactercoretick" << endl;
    cout << charCore.m_HookTick << endl;
    charCore.Tick(true);
    cout << "end charactercoretick" << endl;

    // à faire :
    //character chore ->Tick();
  }


};


CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main(int argc, char**argv){

  cout << "starting test..." << endl;
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

  cout << "end test" << endl;
  return result.wasSuccessful() ? 0 : 1;
}
