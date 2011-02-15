#ifndef MOCK_OBJS_H
#define MOCK_OBJS_H

#include "protocol.h"
#include "network.h"
#include "engine/masterserver.h"
#include "engine/map.h"
#include "snapshot.h"
#include "demo.h"
#include "engine.h"
#include "engine/server/register.h"
#include "engine/console.h"
#include "engine/server.h"
#include "engine/server/server.h"
#include "flag.h"
#include "player.h"

class CGameContext;
class IServer;
class IGameController;
class MockServer;
class CServer;

class MockServer : public CServer
{
public:
  MockServer(){};
  ~MockServer(){};
  virtual int Tick() {
    return 100;
  };
  void setIngame(int index);
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
  MockGameWorld(MockServer *ms);
  
  virtual CGameContext *GameServer() {return msg;}
  virtual IServer *Server(){return srv;}

private :
  CGameContext *msg;
  IServer *srv;
};




class MockController : public IGameController 
{
public:
  MockController(CGameContext *gc);
  ~MockController(){};
  int ClampTeam(int team) { return team;};
};

#endif /* MOCK_OBJS_H */
