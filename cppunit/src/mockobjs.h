#ifndef MOCK_OBJS_H
#define MOCK_OBJS_H

//src/game/server/player.h
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
  MockGameWorld();
  
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
