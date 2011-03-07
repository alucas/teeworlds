#ifdef CONF_UNITTESTING

#include "mockobjs.h"

MockGameWorld::MockGameWorld(MockServer *ms):CGameWorld() {
	msg = new MockGameServer(ms);
	srv = ms;
}


MockController::MockController(CGameContext *gc) : IGameController (gc) {};

void
MockServer::setIngame(int index) {
	m_aClients[0].m_State = CClient::STATE_INGAME;
}

#endif /* CONF_UNITTESTING */
