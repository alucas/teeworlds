/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_WEAPON_H
#define GAME_SERVER_ENTITIES_WEAPON_H

#include <game/server/entity.h>
#include <game/server/entities/pickup.h>

class CWeapon : public CPickup
{
public:
	CWeapon(CGameWorld *pGameWorld, int weaponType);
	
	/*virtual void Reset();
	  virtual void Tick();*/
	virtual int OnPickup(CCharacter *pChr);
};

#endif
