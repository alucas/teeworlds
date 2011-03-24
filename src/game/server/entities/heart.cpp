/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "heart.h"

CHeart::CHeart(CGameWorld *pGameworld)
  : IPowerup(pGameworld,POWERUP_HEALTH)
{
}


int
CHeart::OnPickup(CCharacter *pChr) 
{
	if(pChr->IncreaseHealth(1))
	{
		GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
		return g_pData->m_aPickups[m_Type].m_Respawntime;
	}
	return -1;
}
