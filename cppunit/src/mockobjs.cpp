#include "mockobjs.h"

MockGameWorld::MockGameWorld() {
    msg = new MockGameServer();
    srv = new MockServer();
}


MockController::MockController(CGameContext *gc) : IGameController (gc) {};
