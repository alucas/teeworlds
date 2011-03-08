/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "weapon.h"

CWeapon::CWeapon(CGameWorld *pGameworld, int weaponType)
: CPickup(pGameworld,POWERUP_WEAPON,weaponType)
{
}

int
CWeapon::OnPickup(CCharacter *pChr) 
{
	if(pChr->GiveWeapon(m_Subtype, 10))
	{
		//RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;
		
		if(m_Subtype == WEAPON_GRENADE)
			GameServer()->CreateSound(m_Pos, SOUND_PICKUP_GRENADE);
		else if(m_Subtype == WEAPON_SHOTGUN)
			GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN);
		else if(m_Subtype == WEAPON_RIFLE)
			GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN);
		
		if(pChr->GetPlayer())
			GameServer()->SendWeaponPickup(pChr->GetPlayer()->GetCID(), m_Subtype);
		return g_pData->m_aPickups[m_Type].m_Respawntime;
	}
	return -1;
}
