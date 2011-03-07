#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "mockobjs.h"
#include <stdio.h>
#include <string.h>

using namespace std;


void 
setUpMockInput(CNetObj_PlayerInput *input) {
	mockInput.m_Direction = 1;
	mockInput.m_TargetX = 15;
	mockInput.m_TargetY = 15;
	mockInput.m_Jump = 0;
	mockInput.m_Fire = 1;
	mockInput.m_Hook = 0;
	mockInput.m_PlayerState = PLAYERSTATE_PLAYING;
	mockInput.m_WantedWeapon = 0;
	mockInput.m_NextWeapon = 0;
	mockInput.m_PrevWeapon = 0;
}


class Test : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(Test);
	CPPUNIT_TEST(testSnap);
	CPPUNIT_TEST(testFireWeapon);
	CPPUNIT_TEST_SUITE_END();

	MockGameWorld *gw;
	//CEntity *entity;
	MockGameServer *mgs;
	MockServer *ms;
    
    
public:
	void setUp(void) {
	
		ms = new MockServer();
		gw = new MockGameWorld(ms);
		mgs = new MockGameServer(ms);
		//    g_Config = *CreateConfig();
		mgs->m_pController = new MockController(mgs);
		mgs->m_World.SetGameServer(mgs);
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

	void testFireWeapon(void) {
		CCharacter *character = new (1) CCharacter(&mgs->m_World);//gw);
		CPlayer *player = new (1) CPlayer(mgs,1,1);

		vec2 spawnPos = vec2(10.f,10.f);
		character->Spawn(player,vec2(10,10));
		/*character->m_Pos.x = 10;
		  character->m_Pos.y = 10;*/
		CNetObj_PlayerInput mockInput;
		memset(&mockInput,0,sizeof(mockInput));
  
		setUpInput(&mockInput);
    
    
		character->GiveWeapon(WEAPON_GRENADE,10);
		character->SetWeapon(WEAPON_GRENADE);

		character->OnDirectInput(&mockInput);

		character->FireWeapon();
		delete character;
	}


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
