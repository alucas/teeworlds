//--- Hello, World! for CppUnit

#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>


#include "player.h"


class CGameContext;
class IServer;
class IGameController;
class MockServer;


class MockServer : public IServer
{
public:
  MockServer(){};
  ~MockServer(){};
  virtual int Tick() {
    return 100;
  };

  virtual void SetClientName(int ClientID, const char *pName){};
  virtual void SetClientScore(int ClientID, int Score){};
  virtual void SetBrowseInfo(const char *pGameType, int Progression){};
  virtual int SendMsg(CMsgPacker *pMsg, int Flags, int ClientId){return 0;};    

  virtual int SnapNewID(){return 0;};
  virtual void SnapFreeID(int ID){};
  virtual void *SnapNewItem(int Type, int Id, int Size){};


  virtual const char* ClientName(int){};
  virtual bool ClientIngame(int) {return true;};
  virtual int GetClientInfo(int, IServer::CClientInfo*) {return 0;};
  virtual void GetClientIP(int, char*, int) {};
  virtual int* LatestInput(int, int*) {int i = 0;return  &i;};
  virtual void SnapSetStaticsize(int, int) {};
  virtual bool IsAuthed(int) {return true;};
  virtual void Kick(int, const char*){};
};

class MockGameServer : public CGameContext
{
public:
  MockGameServer() : CGameContext(){
  };

  ~MockGameServer(){

  };
};

class MockGameWorld : public CGameWorld 
{
public :
  MockGameWorld() {

    msg = new MockGameServer();
    srv = new MockServer();
  };
  
  virtual CGameContext *GameServer() {return msg;}
  virtual IServer *Server(){return srv;}

private :
  CGameContext *msg;
  IServer *srv;
};




class MockController : public IGameController 
{
public:
  MockController(CGameContext *gc) : IGameController (gc) {};
  ~MockController(){};
  int ClampTeam(int team) { return team;};
};


class Test : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(testPlayer);
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
  void testPlayer(void) {
    entity->Tick();
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
