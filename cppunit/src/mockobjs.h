#ifdef CONF_UNITTESTING

#ifndef MOCK_OBJS_H
#define MOCK_OBJS_H

#include <iostream>

#include "engine/shared/protocol.h"
#include "engine/shared/network.h"
#include "engine/shared/snapshot.h"
#include "engine/shared/demo.h"
#include "engine/shared/engine.h"

#include "engine/masterserver.h"
#include "engine/map.h"
#include "engine/console.h"
#include "engine/server.h"

#include "engine/server/server.h"
#include "engine/server/register.h"

#include "game/server/player.h"

#include "game/server/entities/flag.h"


class CGameContext;
class IServer;
class IGameController;
class MockServer;
class CServer;

class MockServer : public CServer
{
public:
  MockServer(){};
  ~MockServer(){
  };
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
  ~MockGameWorld() {

  }


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
  ~MockController(){
 
  };
  int ClampTeam(int team) { return team;};
};

#endif /* MOCK_OBJS_H */
#endif /* CONF_UNITTESTING */
