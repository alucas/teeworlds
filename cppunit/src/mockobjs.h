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

#include "engine/shared/config.h"
#include "engine/config.h"
#include "engine/shared/compression.h"

#include "engine/server/register.h"
#include "game/server/player.h"
#include "game/server/entities/flag.h"

using namespace std;

class CGameContext;
class IServer;
class IGameController;
class MockServer;
class CServer;

class MockServer : public CServer
{
public:
	MockServer(){}
	~MockServer(){}

	virtual int Tick();
	virtual int SendMsg(CMsgPacker *pMsg, int Flags, int ClientID);
	virtual void setIngame(int index);
};

class MockGameServer : public CGameContext
{
	MockServer *m_pMockServer;
public:
	~MockGameServer(){}

	MockGameServer(MockServer *mockServ);

	virtual IServer * Server() const { return m_pMockServer;}
};

class MockGameWorld : public CGameWorld 
{
	CGameContext *m_pMockGameServer;
	IServer *m_pServer;

public :
	MockGameWorld(MockServer *m_pMockServer);
	~MockGameWorld(){}

	virtual CGameContext *GameServer() {return m_pMockGameServer;}
	virtual IServer *Server(){return m_pServer;}
};




class MockController : public IGameController 
{
public:
	MockController(CGameContext *gc);
	~MockController(){}

	virtual void OnCharacterSpawn(class CCharacter *pChr);
	virtual int ClampTeam(int team);
};

#endif /* MOCK_OBJS_H */
#endif /* CONF_UNITTESTING */
