/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "powerup.h"

IPowerup::IPowerup(CGameWorld *pGameworld, int type)
  : CPickup(pGameworld,type,0)
{
}
