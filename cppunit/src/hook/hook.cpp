//This class tests the hook behavior.
#include <iostream>
#include <math.h>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>


//Tested file, that contains CCharacterCore class.
#include "gamecore.h"

#include "mockCollision.h"
#include "mockWorldCore.h"
#include "protocol.h"

using namespace std;

class Test : public CPPUNIT_NS::TestCase
{

  CCharacterCore charCore;
  CWorldCore *pWorld;
  MockCollision *pCollision;

  struct CNetObj_PlayerInput playerInput;

  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(CharacterCoreTick);

  CPPUNIT_TEST_SUITE_END();

public:

  void setUp(void) {

    pWorld = new MockWorldCore();
    pCollision = new MockCollision();

    charCore.Init(pWorld, pCollision);
    charCore.Reset();

    charCore.m_HookState = HOOK_IDLE;

    playerInput.m_Hook = 0;
    charCore.m_Input = playerInput;

   
  }

  void tearDown(void) {
       
  }

protected:

  //Test all automaton's transitions.
  void CharacterCoreTick(void) {

    playerInput.m_Hook= 0;

    charCore.Tick(true);
    CPPUNIT_ASSERT(charCore.GetHookState() == HOOK_IDLE);

    charCore.m_Input.m_Hook= 1;

    charCore.Tick(true);
    CPPUNIT_ASSERT(charCore.GetHookState() == HOOK_FLYING);


    charCore.setHookPosition(4,3);    
    charCore.setHookDirection(4,3);    
 
    charCore.Tick(true);    
    CPPUNIT_ASSERT(charCore.GetHookState() == HOOK_RETRACT_START);

    charCore.Tick(true); 
    charCore.Tick(true);    

    CPPUNIT_ASSERT(charCore.GetHookState() == HOOK_RETRACT_END);

    charCore.Tick(true);    
    CPPUNIT_ASSERT(charCore.GetHookState() == HOOK_RETRACTED);

    charCore.m_Input.m_Hook= 0;
    charCore.Tick(true);    
    CPPUNIT_ASSERT(charCore.GetHookState() == HOOK_IDLE);
  }

};


CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main(int argc, char**argv){

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
