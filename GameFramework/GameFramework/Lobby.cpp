//#include "stdafx.h"
#include "Lobby.h"
#include "Player.h"
#include <GameInput.h>
#include "ValueData.h"
#include "MapEditor.h"
#include "Background.h"


Lobby::Lobby()
{
	m_NextLevel = CVector::MapIndex({ 1,2 });
}
Lobby::~Lobby()
{
}

void Lobby::Loading()
{
	// �������� ���ڿ� new �ϸ�? �����ڰ� ����� ���ÿ� �� 
	SetPlayer(new Player());
	Background* NewBack = new Background();
	m_LobbyMap = new MapEditor(L"Lobby.DATA");
	LobbySound = GameSound::SoundPlay(L"lobby.mp3",-1);
	LobbySound->Paused();
	//InitUI();
}

void Lobby::StateUpdate()
{
	//GameState::ChangeState(L"PLAY");
	if (GetPlayer()->OnStair() && GetPlayer()->MapIndex() == m_NextLevel)
	{
		ChangeState(L"LEVEL1");
		LobbySound->Stop();
		GetPlayer()->Death();
		m_LobbyMap->Death();
		TurnOffUI();
	}
}

void Lobby::SoundOn()
{
	LobbySound->ReStart();
}