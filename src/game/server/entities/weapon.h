/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_WEAPON_H
#define GAME_SERVER_ENTITIES_WEAPON_H

#include <game/server/entity.h>
#include <game/server/entities/pickup.h>
#include "character.h"

class IWeapon 
{
public:
  IWeapon(CGameWorld *pGameWorld,CCharacter *pCarrier, int weaponType);
	
	int WeaponType() const { return m_Type;}

	virtual int FireWeapon(vec2 startPos, vec2 direction);
	virtual bool CanFire();
	virtual void RegenAmmo(int ReloadTimer);
	
	virtual CGameWorld *GameWorld() { return m_pGameWorld;}
	virtual CGameContext *GameServer(){return GameWorld()->GameServer();}
	virtual IServer *Server(){return GameWorld()->Server();}

	int m_Type;
	int m_AmmoRegenStart;
	int m_AmmoRegenTime;
	int m_AmmoRegenValue;

	int m_Ammo;
	int m_Ammocost;
	int m_Maxammo;
	int m_Damage;
	
	bool m_FullAuto;
	bool m_Got;
	
	CGameWorld *m_pGameWorld;
	CCharacter *m_pCarrier;
};


class CGun : public IWeapon
{
public:
	CGun(CGameWorld *pGameWorld, CCharacter *pCarrier);
	virtual int FireWeapon(vec2 startPos, vec2 direction);
};

class CHammer : public IWeapon
{
public:
	CHammer(CGameWorld *pGameWorld, CCharacter *pCarrier);
	virtual int FireWeapon(vec2 startPos, vec2 direction);
};

class CNinja : public IWeapon
{
public:
	CNinja(CGameWorld *pGameWorld, CCharacter *pCarrier);
	//virtual int FireWeapon(vec2 startPos, vec2 direction);
};

class CLaserRifle : public IWeapon
{
public:
	CLaserRifle(CGameWorld *pGameWorld, CCharacter *pCarrier);
	virtual int FireWeapon(vec2 startPos, vec2 direction);
};

class CShotgun : public IWeapon
{
public:
	CShotgun(CGameWorld *pGameWorld, CCharacter *pCarrier);
	virtual int FireWeapon(vec2 startPos, vec2 direction);
};

class CGrenadeLauncher : public IWeapon
{
public:
	CGrenadeLauncher(CGameWorld *pGameWorld, CCharacter *pCarrier);
	virtual int FireWeapon(vec2 startPos, vec2 direction);
};

#endif
