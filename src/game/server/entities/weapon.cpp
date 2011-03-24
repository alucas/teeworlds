/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "projectile.h"
#include "character.h"
#include "laser.h"
#include "weapon.h"

IWeapon::IWeapon(CGameWorld *pGameworld, CCharacter *pCarrier, int WeaponType)
{
	m_AmmoRegenStart = -1;
	m_AmmoRegenTime = g_pData->m_Weapons.m_aId[WeaponType].m_Ammoregentime;

//TODO put a field AmmoRegenValue in client/server_data
	m_AmmoRegenValue = 1;

	m_Maxammo = g_pData->m_Weapons.m_aId[WeaponType].m_Maxammo;
	m_Ammo = 0;
	m_Ammocost = 1;
	m_Damage = g_pData->m_Weapons.m_aId[WeaponType].m_Damage;
	
	m_Got = false;
	m_FullAuto = false;
	
	m_Type = WeaponType;

	m_pGameWorld = pGameworld;
	m_pCarrier = pCarrier;
}

CGun::CGun(CGameWorld *pGameworld, CCharacter *pCarrier) : IWeapon(pGameworld,pCarrier,WEAPON_GUN) 
{
}

CNinja::CNinja(CGameWorld *pGameworld, CCharacter *pCarrier) : IWeapon(pGameworld,pCarrier,WEAPON_NINJA)
{
}

CHammer::CHammer(CGameWorld *pGameworld, CCharacter *pCarrier) : IWeapon(pGameworld,pCarrier,WEAPON_HAMMER)
{
	m_Ammo = -1;
	m_Maxammo = -1;
}

CGrenadeLauncher::CGrenadeLauncher(CGameWorld *pGameworld, CCharacter *pCarrier) : IWeapon(pGameworld,pCarrier,WEAPON_GRENADE)
{
	m_FullAuto = true;
}

CShotgun::CShotgun(CGameWorld *pGameworld, CCharacter *pCarrier) : IWeapon(pGameworld,pCarrier,WEAPON_SHOTGUN)
{
	m_FullAuto = true;
}

CLaserRifle::CLaserRifle(CGameWorld *pGameworld, CCharacter *pCarrier) : IWeapon(pGameworld,pCarrier,WEAPON_RIFLE)
{
	m_FullAuto = true;
}

int
IWeapon::FireWeapon(vec2 startPos, vec2 Direction)
{
	if (m_Ammo > 0)
		m_Ammo -= m_Ammocost;
	return g_pData->m_Weapons.m_aId[m_Type].m_Firedelay * Server()->TickSpeed() / 1000;
}

bool
IWeapon::CanFire()
{
	//-1 is infinite ammo
	return m_Ammo == -1 || (m_Ammo > 0 && m_Ammo >= m_Ammocost);
}

void
IWeapon::RegenAmmo(int ReloadTimer)
{
	if(m_AmmoRegenTime)
	{
		// If equipped and not active, regen ammo?
		if (ReloadTimer <= 0)
		{
			if (m_AmmoRegenStart < 0)
				m_AmmoRegenStart = Server()->Tick();
			
			if ((Server()->Tick() - m_AmmoRegenStart) >= m_AmmoRegenTime * Server()->TickSpeed() / 1000)
			{
				// Add some ammo
				m_Ammo = min(m_Ammo + m_AmmoRegenValue, m_Maxammo);
				m_AmmoRegenStart = -1;
			}
		}
		else
		{
			m_AmmoRegenStart = -1;
		}
	}
}


int
CHammer::FireWeapon(vec2 ProjStartPos, vec2 Direction)
{
	// reset objects Hit
	//m_NumObjectsHit = 0;
	GameServer()->CreateSound(m_pCarrier->m_Pos, SOUND_HAMMER_FIRE);
			
	CCharacter *apEnts[MAX_CLIENTS];
	int Hits = 0;
	int Num = GameServer()->m_World.FindEntities(ProjStartPos, m_pCarrier->m_ProximityRadius*0.5f, (CEntity**)apEnts, 
						     MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);

	for (int i = 0; i < Num; ++i)
	{
		CCharacter *pTarget = apEnts[i];
				
		if ((pTarget == m_pCarrier) || GameServer()->Collision()->IntersectLine(ProjStartPos, pTarget->m_Pos, NULL, NULL))
			continue;

		// set his velocity to fast upward (for now)
		if(length(pTarget->m_Pos-ProjStartPos) > 0.0f)
			GameServer()->CreateHammerHit(pTarget->m_Pos-normalize(pTarget->m_Pos-ProjStartPos)*m_pCarrier->m_ProximityRadius*0.5f);
		else
			GameServer()->CreateHammerHit(ProjStartPos);
				
		vec2 Dir;
		if (length(pTarget->m_Pos - m_pCarrier->m_Pos) > 0.0f)
			Dir = normalize(pTarget->m_Pos - m_pCarrier->m_Pos);
		else
			Dir = vec2(0.f, -1.f);
					
		pTarget->TakeDamage(vec2(0.f, -1.f) + normalize(Dir + vec2(0.f, -1.1f)) * 10.0f, m_Damage,
				    m_pCarrier->GetPlayer()->GetCID(), m_Type);
		Hits++;
	}
			
	// if we Hit anything, we have to wait for the reload
	if(Hits)
		return Server()->TickSpeed()/3;
	return g_pData->m_Weapons.m_aId[m_Type].m_Firedelay * Server()->TickSpeed() / 1000;
}


int
CGun::FireWeapon(vec2 ProjStartPos, vec2 Direction) 
{
	CProjectile *pProj = new CProjectile(GameWorld(), WEAPON_GUN,
					     m_pCarrier->GetPlayer()->GetCID(), ProjStartPos,
					     Direction,
					     (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GunLifetime),
					     1, 0, 0, -1, WEAPON_GUN);
	
	// pack the Projectile and send it to the client Directly
	CNetObj_Projectile p;
	pProj->FillInfo(&p);
	
	CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
	Msg.AddInt(1);
	for(unsigned i = 0; i < sizeof(CNetObj_Projectile)/sizeof(int); i++)
				Msg.AddInt(((int *)&p)[i]);
	
	Server()->SendMsg(&Msg, 0, m_pCarrier->GetPlayer()->GetCID());
	
	GameServer()->CreateSound(m_pCarrier->m_Pos, SOUND_GUN_FIRE);

	return IWeapon::FireWeapon(ProjStartPos,Direction);
}

int
CShotgun::FireWeapon(vec2 ProjStartPos, vec2 Direction) 
{
	int ShotSpread = 2;

	CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
	Msg.AddInt(ShotSpread*2+1);
	
	for(int i = -ShotSpread; i <= ShotSpread; ++i)
	{
		float Spreading[] = {-0.185f, -0.070f, 0, 0.070f, 0.185f};
		float a = GetAngle(Direction);
		a += Spreading[i+2];
		float v = 1-(absolute(i)/(float)ShotSpread);
		float Speed = mix((float)GameServer()->Tuning()->m_ShotgunSpeeddiff, 1.0f, v);
		CProjectile *pProj = new CProjectile(GameWorld(), WEAPON_SHOTGUN,
						     m_pCarrier->GetPlayer()->GetCID(),
						     ProjStartPos,
						     vec2(cosf(a), sinf(a))*Speed,
						     (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_ShotgunLifetime),
						     1, 0, 0, -1, WEAPON_SHOTGUN);
		
		// pack the Projectile and send it to the client Directly
		CNetObj_Projectile p;
		pProj->FillInfo(&p);
				
		for(unsigned i = 0; i < sizeof(CNetObj_Projectile)/sizeof(int); i++)
			Msg.AddInt(((int *)&p)[i]);
	}
	
	Server()->SendMsg(&Msg, 0,m_pCarrier->GetPlayer()->GetCID());					
	
	GameServer()->CreateSound(m_pCarrier->m_Pos, SOUND_SHOTGUN_FIRE);
	return IWeapon::FireWeapon(ProjStartPos,Direction);
}

int
CGrenadeLauncher::FireWeapon(vec2 ProjStartPos, vec2 Direction) 
{
	CProjectile *pProj = new CProjectile(GameWorld(), WEAPON_GRENADE,
					     m_pCarrier->GetPlayer()->GetCID(),
					     ProjStartPos,
					     Direction,
					     (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GrenadeLifetime),
					     1, true, 0, SOUND_GRENADE_EXPLODE, WEAPON_GRENADE);

	// pack the Projectile and send it to the client Directly
	CNetObj_Projectile p;
	pProj->FillInfo(&p);
			
	CMsgPacker Msg(NETMSGTYPE_SV_EXTRAPROJECTILE);
	Msg.AddInt(1);
	for(unsigned i = 0; i < sizeof(CNetObj_Projectile)/sizeof(int); i++)
		Msg.AddInt(((int *)&p)[i]);
	Server()->SendMsg(&Msg, 0, m_pCarrier->GetPlayer()->GetCID());
			
	GameServer()->CreateSound(m_pCarrier->m_Pos, SOUND_GRENADE_FIRE);
	return IWeapon::FireWeapon(ProjStartPos,Direction);
}

int
CLaserRifle::FireWeapon(vec2 ProjStartPos, vec2 Direction) 
{
	new CLaser(GameWorld(), m_pCarrier->m_Pos, Direction, GameServer()->Tuning()->m_LaserReach, m_pCarrier->GetPlayer()->GetCID());
	GameServer()->CreateSound(m_pCarrier->m_Pos, SOUND_RIFLE_FIRE);
	return IWeapon::FireWeapon(ProjStartPos,Direction);
}
/*int
CNinja::FireWeapon(vec2 ProjStartPos, vec2 Direction) 
{
	// reset Hit objects
	//m_NumObjectsHit = 0;
	
	m_AttackTick = Server()->Tick();
	m_Ninja.m_ActivationDir = Direction;
	m_Ninja.m_CurrentMoveTime = g_pData->m_Weapons.m_Ninja.m_Movetime * Server()->TickSpeed() / 1000;
	
	GameServer()->CreateSound(m_pCarrier->m_Pos, SOUND_NINJA_FIRE);
	}*/
