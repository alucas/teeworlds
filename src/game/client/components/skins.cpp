/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <math.h>

#include <base/system.h>
#include <base/math.h>

#include <engine/graphics.h>
#include <engine/storage.h>
#include <engine/shared/engine.h>

#include "skins.h"

#define IMAGE_EXTENSION_STRING ".png"
#define IMAGE_EXTENSION_SIZE ((int)(sizeof(IMAGE_EXTENSION_STRING)/sizeof(char)) - 1)

#define RED   0
#define GREEN 1
#define BLUE  2
#define ALPHA 3

void CSkins::SkinScan(const char *pName, int IsDir, int DirType, void *pUser)
{
	CSkins *pSelf = (CSkins *)pUser;
	int NameLength = str_length(pName);
	if(IsDir || NameLength <= IMAGE_EXTENSION_SIZE || str_comp(pName + NameLength - IMAGE_EXTENSION_SIZE, IMAGE_EXTENSION_STRING) != 0)
		return;
		
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/%s", pName);
	CImageInfo Info;
	if(!pSelf->Graphics()->LoadPNG(&Info, aBuf, DirType))
	{
		str_format(aBuf, sizeof(aBuf), "failed to load skin from %s", pName);
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "game", aBuf);
		return;
	}

	if(Info.m_Format != CImageInfo::FORMAT_RGBA)
	{
		mem_free(Info.m_pData);

		str_format(aBuf, sizeof(aBuf), "the skin from %s doesn't have alpha channel", pName);
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "game", aBuf);
		return;
	}
	
	CSkin Skin;
	Skin.m_OrgTexture = pSelf->Graphics()->LoadTextureRaw(Info.m_Width, Info.m_Height, Info.m_Format, Info.m_pData, Info.m_Format, 0);
	
	int BodySize = 96; // body size
	unsigned char *pData = (unsigned char *)Info.m_pData;
	int Step = 4;
	int Pitch = Info.m_Width*Step;

	// dig out blood color
	{
		int aColors[3] = {0};
		for(int y = 0; y < BodySize; y++)
			for(int x = 0; x < BodySize; x++)
			{
				if(pData[y*Pitch + x*Step + ALPHA] > 128)
				{
					aColors[RED]   += pData[y*Pitch + x*Step + RED];
					aColors[GREEN] += pData[y*Pitch + x*Step + GREEN];
					aColors[BLUE]  += pData[y*Pitch + x*Step + BLUE];
				}
			}
			
		Skin.m_BloodColor = normalize(vec3(aColors[RED], aColors[GREEN], aColors[BLUE]));
	}
	
	// make the texture gray scale
	for(int i = 0; i < Info.m_Width*Info.m_Height; i++)
	{
		int v = (pData[i*Step + RED] + pData[i*Step + GREEN] + pData[i*Step + BLUE])/3;
		pData[i*Step + RED]   = v;
		pData[i*Step + GREEN] = v;
		pData[i*Step + BLUE]  = v;
	}

	
	int Freq[256] = {0};
	int OrgWeight = 0;
	int NewWeight = 192;
		
	// find most common frequence
	for(int y = 0; y < BodySize; y++)
		for(int x = 0; x < BodySize; x++)
		{
			if(pData[y*Pitch + x*Step + ALPHA] > 128)
				Freq[pData[y*Pitch + x*Step]]++;
		}
		
	for(int i = 1; i < 256; i++)
	{
		if(Freq[OrgWeight] < Freq[i])
			OrgWeight = i;
	}

	// reorder
	int InvOrgWeight = 255-OrgWeight;
	int InvNewWeight = 255-NewWeight;
	for(int y = 0; y < BodySize; y++)
		for(int x = 0; x < BodySize; x++)
		{
			int v = pData[y*Pitch + x*Step];
			if(v <= OrgWeight)
				v = (int)(((v/(float)OrgWeight) * NewWeight));
			else
				v = (int)(((v-OrgWeight)/(float)InvOrgWeight)*InvNewWeight + NewWeight);

			pData[y*Pitch + x*Step + RED]   = v;
			pData[y*Pitch + x*Step + GREEN] = v;
			pData[y*Pitch + x*Step + BLUE]  = v;
		}
	
	Skin.m_ColorTexture = pSelf->Graphics()->LoadTextureRaw(Info.m_Width, Info.m_Height, Info.m_Format, Info.m_pData, Info.m_Format, 0);
	mem_free(Info.m_pData);

	// set skin data	
	str_copy(Skin.m_aName, pName, min((int)sizeof(Skin.m_aName), NameLength - IMAGE_EXTENSION_SIZE + 1));
	str_format(aBuf, sizeof(aBuf), "load skin %s", Skin.m_aName);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_ADDINFO, "game", aBuf);
	pSelf->m_aSkins.add(Skin);
}


void CSkins::OnInit()
{
	// load skins
	m_aSkins.clear();
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins", SkinScan, this);
	if(!m_aSkins.size())
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "gameclient", "failed to load skins. folder='skins/'");
		CSkin DummySkin;
		DummySkin.m_OrgTexture = -1;
		DummySkin.m_ColorTexture = -1;
		str_copy(DummySkin.m_aName, "dummy", sizeof(DummySkin.m_aName));
		DummySkin.m_BloodColor = vec3(1.0f, 1.0f, 1.0f);
		m_aSkins.add(DummySkin);
	}
}

int CSkins::Num()
{
	return m_aSkins.size();	
}

const CSkins::CSkin *CSkins::Get(int Index)
{
	return &m_aSkins[Index%m_aSkins.size()];
}

int CSkins::Find(const char *pName)
{
	for(int i = 0; i < m_aSkins.size(); i++)
	{
		if(str_comp(m_aSkins[i].m_aName, pName) == 0)
			return i;
	}
	return -1;
}

vec3 CSkins::GetColorV3(int v)
{
	return HslToRgb(vec3(((v>>16)&0xff)/255.0f, ((v>>8)&0xff)/255.0f, 0.5f+(v&0xff)/255.0f*0.5f));
}

vec4 CSkins::GetColorV4(int v)
{
	vec3 r = GetColorV3(v);
	return vec4(r.r, r.g, r.b, 1.0f);
}
