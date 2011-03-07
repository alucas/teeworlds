#ifdef CONF_UNITTESTING
#ifndef HOOK_TEST
#define HOOK_TEST

//This class tests the hook behavior.
#include <iostream>
#include <math.h>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "mockobjs.h"

using namespace std;

class HookTest : public CppUnit::TestFixture
{
	CCharacterCore m_charCore;
	CWorldCore *m_pWorld;
	MockCollision *m_pCollision;

	struct CNetObj_PlayerInput playerInput;

	CPPUNIT_TEST_SUITE(HookTest);
	CPPUNIT_TEST(CharacterCoreTick);

	CPPUNIT_TEST_SUITE_END();

public:

	void setUp(void) {

		m_pWorld = new MockWorldCore();
		m_pCollision = new MockCollision();

		m_charCore.Init(m_pWorld, m_pCollision);
		m_charCore.Reset();

		m_charCore.m_HookState = HOOK_IDLE;

		playerInput.m_Hook = 0;
		m_charCore.m_Input = playerInput;

   
	}

	void tearDown(void) {
       
	}

protected:

	//Test all automaton's transitions.
	void CharacterCoreTick(void) {

		playerInput.m_Hook= 0;

		m_charCore.Tick(true);
		CPPUNIT_ASSERT(m_charCore.GetHookState() == HOOK_IDLE);

		m_charCore.m_Input.m_Hook= 1;

		m_charCore.Tick(true);
		CPPUNIT_ASSERT(m_charCore.GetHookState() == HOOK_FLYING);


		m_charCore.setHookPosition(4,3);    
		m_charCore.setHookDirection(4,3);    
 
		m_charCore.Tick(true);    
		CPPUNIT_ASSERT(m_charCore.GetHookState() == HOOK_RETRACT_START);

		m_charCore.Tick(true); 
		m_charCore.Tick(true);    

		CPPUNIT_ASSERT(m_charCore.GetHookState() == HOOK_RETRACT_END);

		m_charCore.Tick(true);    
		CPPUNIT_ASSERT(m_charCore.GetHookState() == HOOK_RETRACTED);

		m_charCore.m_Input.m_Hook= 0;
		m_charCore.Tick(true);    
		CPPUNIT_ASSERT(m_charCore.GetHookState() == HOOK_IDLE);
	}

};
#endif
#endif
