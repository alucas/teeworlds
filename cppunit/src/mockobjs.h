#ifndef MOCK_OBJS_H
#define MOCK_OBJS_H

#include <iostream>

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
#include "engine/shared/config.h"
#include "engine/config.h"
#include "engine/shared/compression.h"

#include "player.h"

using namespace std;

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

	virtual int SendMsg(CMsgPacker *pMsg, int Flags, int ClientID) {
		int i,size;

		int data[20];
		size = CVariableInt::Decompress(pMsg->Data(),pMsg->Size(),data);
		size /= sizeof(int);
    
		cout << "SIZE = " << size << "\n";
		cout << "\nDATA = ";
		for (i = 0; i < size; i++) {
			cout << data[i] << " ";

		}
		cout << endl;

		/*    cout << "\nSIZE = " << size
		      << "\nDATA = ";
    
		      for (i = 0; i < 8;i++) {
		      data = 0;
		      CVariableInt::Unpack(&(pMsg->Data()[i]), &data);
		      cout << data<< " ";
		      }
		      cout << endl;*/
	}

	void setIngame(int index);
};

class MockGameServer : public CGameContext
{
public:
	~MockGameServer(){
	};
  
MockGameServer(MockServer *mockServ) : CGameContext(){
		ms = mockServ;
	};

	virtual IServer * Server() const { return ms;}
	//CGameWorld


private:
	MockServer *ms;
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

	void OnCharacterSpawn(class CCharacter *pChr) { return;}

	int ClampTeam(int team) { return team;};
};

#endif /* MOCK_OBJS_H */
