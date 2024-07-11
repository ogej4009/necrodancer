//#include "stdafx.h"
#include "Level1.h"
#include "Player.h"
#include "MapEditor.h"
#include "Background.h"
#include "Monster.h"


Level1::Level1()
{
	m_NextLevel = CVector::MapIndex({21,-25});
}


Level1::~Level1()
{
}


void Level1::Loading()
{
	///
	SetPlayer(new Player());
	Background* NewBack = new Background();
	m_Level1 = new MapEditor(L"Level1.DATA");
	L1Sound = GameSound::SoundPlay(L"zone1_1.mp3",-1);
	L1Sound->Paused();
	InitUI();
}

void Level1::StateUpdate()
{
	
	if (GetPlayer()->OnStair() && GetPlayer()->MapIndex() == m_NextLevel)
	{
		GetPlayer()->ChangeGlobal();
		ChangeState(L"LEVEL2");
		L1Sound->Stop();
		TurnOffUI();
	}
}

void Level1::SoundOn()
{
	L1Sound->ReStart();
}