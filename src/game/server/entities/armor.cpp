/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "armor.h"

CArmor::CArmor(CGameWorld *pGameworld)
  : IPowerup(pGameworld,POWERUP_ARMOR)
{
}


int
CArmor::OnPickup(CCharacter *pChr) 
{
	if(pChr->IncreaseArmor(1))
	{
		GameServer()->CreateSound(m_Pos, SOUND_PICKUP_ARMOR);
		return g_pData->m_aPickups[m_Type].m_Respawntime;
	}
	return -1;
}
