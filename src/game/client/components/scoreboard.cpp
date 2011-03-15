/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/demo.h>
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/generated/client_data.h>
#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/client/render.h>
#include <game/client/components/motd.h>
#include <game/localization.h>
#include "scoreboard.h"
#include "base/tl/color.h"


CScoreboard::CScoreboard()
{
	OnReset();
}

void CScoreboard::ConKeyScoreboard(IConsole::IResult *pResult, void *pUserData)
{
	((CScoreboard *)pUserData)->m_Active = pResult->GetInteger(0) != 0;
}

void CScoreboard::OnReset()
{
	m_Active = false;
}

void CScoreboard::OnRelease()
{
	m_Active = false;
}

void CScoreboard::OnConsoleInit()
{
	Console()->Register("+scoreboard", "", CFGFLAG_CLIENT, ConKeyScoreboard, this, "Show scoreboard");
}

void CScoreboard::RenderGoals(float x, float y, float w)
{
	float h = 50.0f;

	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.5f);
	RenderTools()->DrawRoundRect(x-10.f, y-10.f, w, h, 10.0f);
	Graphics()->QuadsEnd();

	// render goals
	if(m_pClient->m_Snap.m_pGameobj)
	{
		if(m_pClient->m_Snap.m_pGameobj->m_ScoreLimit)
		{
			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), "%s: %d", Localize("Score limit"), m_pClient->m_Snap.m_pGameobj->m_ScoreLimit);
			TextRender()->Text(0, x, y, 20.0f, aBuf, -1);
		}
		if(m_pClient->m_Snap.m_pGameobj->m_TimeLimit)
		{
			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), Localize("Time limit: %d min"), m_pClient->m_Snap.m_pGameobj->m_TimeLimit);
			TextRender()->Text(0, x+220.0f, y, 20.0f, aBuf, -1);
		}
		if(m_pClient->m_Snap.m_pGameobj->m_RoundNum && m_pClient->m_Snap.m_pGameobj->m_RoundCurrent)
		{
			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), "%s %d/%d", Localize("Round"), m_pClient->m_Snap.m_pGameobj->m_RoundCurrent, m_pClient->m_Snap.m_pGameobj->m_RoundNum);
			float tw = TextRender()->TextWidth(0, 20.0f, aBuf, -1);
			TextRender()->Text(0, x+w-tw-20.0f, y, 20.0f, aBuf, -1);
		}
	}
}

void CScoreboard::RenderSpectators(float x, float y, float w)
{
	char aBuffer[1024*4];
	int Count = 0;
	float h = 120.0f;
	
	str_format(aBuffer, sizeof(aBuffer), "%s: ", Localize("Spectators"));

	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.5f);
	RenderTools()->DrawRoundRect(x-10.f, y-10.f, w, h, 10.0f);
	Graphics()->QuadsEnd();
	
	for(int i = 0; i < Client()->SnapNumItems(IClient::SNAP_CURRENT); i++)
	{
		IClient::CSnapItem Item;
		const void *pData = Client()->SnapGetItem(IClient::SNAP_CURRENT, i, &Item);

		if(Item.m_Type == NETOBJTYPE_PLAYERINFO)
		{
			const CNetObj_PlayerInfo *pInfo = (const CNetObj_PlayerInfo *)pData;
			if(pInfo->m_Team == TEAM_SPECTATORS)
			{
				if(Count)
					str_append(aBuffer, ", ", sizeof(aBuffer));
				str_append(aBuffer, m_pClient->m_aClients[pInfo->m_ClientID].m_aName, sizeof(aBuffer));
				Count++;
			}
		}
	}
	
	TextRender()->Text(0, x+10, y, 32, aBuffer, (int)w-20);
}

void CScoreboard::RenderScoreboard(float x, float y, float w, float h, int Team, const char *pTitle)
{
	if(Team == TEAM_SPECTATORS)
		return;

	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0,0,0,0.5f);
	RenderTools()->DrawRoundRect(x-10.f, y, w, h, 17.0f);
	Graphics()->QuadsEnd();

	// render title
	if(!pTitle)
	{
		if(m_pClient->m_Snap.m_pGameobj->m_GameOver)
			pTitle = Localize("Game over");
		else
			pTitle = Localize("Score board");
	}

	float tw = TextRender()->TextWidth(0, 48, pTitle, -1);
	TextRender()->Text(0, x+10, y, 48, pTitle, -1);

	if(m_pClient->m_Snap.m_pGameobj)
	{
		char aBuf[128];
		int Score = m_pClient->m_Snap.m_pGameobj->m_Teamscore[Team];
		str_format(aBuf, sizeof(aBuf), "%d", Score);
		tw = TextRender()->TextWidth(0, 48, aBuf, -1);
		TextRender()->Text(0, x+w-tw-30, y, 48, aBuf, -1);
	}

	y += 54.0f;

	// render headlines
	TextRender()->Text(0, x+10, y, 24.0f, Localize("Score"), -1);
	TextRender()->Text(0, x+125, y, 24.0f, Localize("Name"), -1);
	TextRender()->Text(0, x+w-70, y, 24.0f, Localize("Ping"), -1);
	y += 29.0f;

	float FontSize = 35.0f;
	float LineHeight = 50.0f;
	float TeeSizeMod = 1.0f;
	float TeeOffset = 0.0f;
	
	if(m_pClient->m_Snap.m_aTeamSize[Team] > 13)
	{
		FontSize = 30.0f;
		LineHeight = 40.0f;
		TeeSizeMod = 0.8f;
		TeeOffset = -5.0f;
	}
	
	// render player scores
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		const CNetObj_PlayerInfo *pInfo = m_pClient->m_Snap.m_paInfoByScore[i];
		if(!pInfo || pInfo->m_Team != Team)
			continue;

		// make sure that we render the correct team

		char aBuf[128];
		if(pInfo->m_Local)
		{
			// background so it's easy to find the local player
			Graphics()->TextureSet(-1);
			Graphics()->QuadsBegin();
			Graphics()->SetColor(1,1,1,0.25f);
			RenderTools()->DrawRoundRect(x, y, w-20, LineHeight*0.95f, 17.0f);
			Graphics()->QuadsEnd();
		}

		float FontSizeResize = FontSize;
		float Width;
		const float ScoreWidth = 60.0f;
		const float PingWidth = 60.0f;
		str_format(aBuf, sizeof(aBuf), "%d", clamp(pInfo->m_Score, -9999, 9999));
		while((Width = TextRender()->TextWidth(0, FontSizeResize, aBuf, -1)) > ScoreWidth)
			--FontSizeResize;
		TextRender()->Text(0, x+ScoreWidth-Width, y+(FontSize-FontSizeResize)/2, FontSizeResize, aBuf, -1);
		
		FontSizeResize = FontSize;
		while(TextRender()->TextWidth(0, FontSizeResize, m_pClient->m_aClients[pInfo->m_ClientID].m_aName, -1) > w-163.0f-PingWidth)
			--FontSizeResize;
		TextRender()->Text(0, x+128.0f, y+(FontSize-FontSizeResize)/2, FontSizeResize, m_pClient->m_aClients[pInfo->m_ClientID].m_aName, -1);

		FontSizeResize = FontSize;
		str_format(aBuf, sizeof(aBuf), "%d", clamp(pInfo->m_Latency, -9999, 9999));
		while((Width = TextRender()->TextWidth(0, FontSizeResize, aBuf, -1)) > PingWidth)
			--FontSizeResize;
		TextRender()->Text(0, x+w-35.0f-Width, y+(FontSize-FontSizeResize)/2, FontSizeResize, aBuf, -1);

		// render avatar
		for(int i = 0; i < m_pClient->m_Snap.m_pGameobj->m_NumberTeams; i++)
			if(m_pClient->m_Snap.m_paFlags[i] && m_pClient->m_Snap.m_paFlags[i]->m_CarriedBy == pInfo->m_ClientID)
			{
				IGraphics::CQuadItem QuadItem(x+55, y-15, 64.0f/2, 64.0f);

				vec4 FlagColor = HslToRgbV4(RenderTools()->GetTeamColorHSL(m_pClient->m_Snap.m_paFlags[i]->m_Team));

				RenderTools()->RenderFlag(&QuadItem, 0.0f, FlagColor, SPRITE_FLAG_FLIP_X);
			}
		
		CTeeRenderInfo TeeInfo = m_pClient->m_aClients[pInfo->m_ClientID].m_RenderInfo;
		TeeInfo.m_Size *= TeeSizeMod;
		RenderTools()->RenderTee(CAnimState::GetIdle(), &TeeInfo, EMOTE_NORMAL, vec2(1,0), vec2(x+90, y+28+TeeOffset));

		
		y += LineHeight;
	}
}

void CScoreboard::RenderRecordingNotification(float x)
{
	if(!m_pClient->DemoRecorder()->IsRecording())
		return;

	//draw the box
	Graphics()->BlendNormal();
	Graphics()->TextureSet(-1);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.4f);
	RenderTools()->DrawRoundRectExt(x, 0.0f, 120.0f, 50.0f, 15.0f, CUI::CORNER_B);
	Graphics()->QuadsEnd();

	//draw the red dot
	Graphics()->QuadsBegin();
	Graphics()->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	RenderTools()->DrawRoundRect(x+20, 15.0f, 20.0f, 20.0f, 10.0f);
	Graphics()->QuadsEnd();

	//draw the text
	TextRender()->Text(0, x+50.0f, 8.0f, 24.0f, Localize("REC"), -1);
}


static const char *GetTeamName(int num){
	static const char *team_names[] = {"team 1","team 2","team 3","team 4", "Team 5", "Team 6", "Team 7", "Team 8"};
	
	return team_names[num % NUM_TEAMS];
}


void CScoreboard::OnRender()
{
	if(!Active())
		return;
		
	// if the score board is active, then we should clear the motd message aswell
	if(m_pClient->m_pMotd->IsActive())
		m_pClient->m_pMotd->Clear();
	

	float Width = 370*3.0f*Graphics()->ScreenAspect();
	float Height = 400*3.0f;

	Graphics()->MapScreen(0, 0, Width, Height);

	float w = 400.0f;

	if(m_pClient->m_Snap.m_pGameobj && !(m_pClient->m_Snap.m_pGameobj->m_Flags&GAMEFLAG_TEAMS))
		RenderScoreboard(Width/2-w/2, 150.0f, 750.0f, w, 0, 0);
	else //team game
	{
		int i;
		const char *pText;
		int NumTeams = m_pClient->m_Snap.m_pGameobj->m_NumberTeams;
		if(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver)
		{
			int WiningTeam = 0;
			int WiningTeamScore = m_pClient->m_Snap.m_pGameobj->m_Teamscore[0];
			for(i = 0; i<NumTeams;i++)
			{
				if (WiningTeamScore < m_pClient->m_Snap.m_pGameobj->m_Teamscore[i])//don't care about execo
				{
					WiningTeam = i;
					WiningTeamScore = m_pClient->m_Snap.m_pGameobj->m_Teamscore[i];
				}
			}

			char aBuffer[128];
			str_format(aBuffer, 128, "%s wins", GetTeamName(WiningTeam));

			pText= Localize(aBuffer);
      
			float w = TextRender()->TextWidth(0, 86.0f, pText, -1);
			TextRender()->Text(0, Width/2-w/2, 39, 86.0f, pText, -1);
		}
	
		int Space = 20;
		int NbrScorboardPerLine = 4;
		int NbSpaces = NumTeams - 1; //one space between 2 board
		int W;
		float h;

		if(!(NumTeams>NbrScorboardPerLine))
		{
			W = NbSpaces*Space + NumTeams*w;
			h = 750.0f;
			for(i=0;i<NumTeams;i++)
				RenderScoreboard(Width/2-W/2+i*(w+Space),150.0f,w,h,i,Localize(GetTeamName(i)));
		}
		else
		{//display on 2 lines
			h = 325.0f;
			int W = NbSpaces*Space + NbrScorboardPerLine*w;
			for(i=0;i<NbrScorboardPerLine;i++)
				RenderScoreboard(Width/2-W/2+i*(w+Space),150.0f,w,h,i,Localize(GetTeamName(i)));
			for(i=NbrScorboardPerLine;i<NumTeams;i++)
				RenderScoreboard(Width/2-W/2+(i-NbrScorboardPerLine)*(w+Space),150.0f+ h + 10,w,h,i,Localize(GetTeamName(i)));
		}
	}//end "team_game"

	RenderGoals(Width/2-w/2, 150+750+25, w);
	RenderSpectators(Width/2-w/2, 150+750+25+50+25, w);
	RenderRecordingNotification((Width/7)*4);
}

bool CScoreboard::Active()
{
	// if we activly wanna look on the scoreboard	
	if(m_Active)
		return true;
		
	if(m_pClient->m_Snap.m_pLocalInfo && m_pClient->m_Snap.m_pLocalInfo->m_Team != TEAM_SPECTATORS)
	{
		// we are not a spectator, check if we are dead
		if(!m_pClient->m_Snap.m_pLocalCharacter)
			return true;
	}

	// if the game is over
	if(m_pClient->m_Snap.m_pGameobj && m_pClient->m_Snap.m_pGameobj->m_GameOver)
		return true;

	return false;
}
