//#include "stdafx.h"
#include "Title.h"
#include "Player.h"
#include "MapEditor.h"
#include <GameInput.h>
#include "GameRender.h"
#include "GameActor.h"
#include "Lobby.h"


Title::Title()
{
}

Title::~Title()
{
}

void Title::Loading()
{
	// 내 게임 요소들을 만들게 할것이다.
	// CreateActor() 대신 new로 만들면
	// 자동등록.
     m_TitleActor = new GameActor();
	 m_TitleActor->ActorType(EActorTypeIndex::UI);

	 m_Menu = m_TitleActor->CreateRender<GameRender>((int)eTitle::MainMenu);
	 m_Continue = m_TitleActor->CreateRender<GameRender>((int)eTitle::Continue);

	 m_Menu->Image(L"mainmenu.bmp");
	 m_Menu->SubPos({ 480,270 });
	 m_Menu->SubSize({ 960,540});

	 m_Continue->Image(L"continue.bmp");
	 m_Continue->SubPos({ 500,500 });
	 m_Continue->SubSize({340,40});

	 TitleSound = GameSound::SoundPlay(L"intro.mp3");
	 TitleSound->Position(120);

	 //타이틀 집어넣으면 밑에줄은 빼야됨
	 TitleSound->Stop();
}

void Title::StateUpdate()
{
	if (GameInput::Down(L"ChangeScene"))
	{
		GameState::ChangeState(L"LOBBY");
		TitleSound->Stop();
	}

}

void Title::SoundOn()
{
	TitleSound = GameSound::SoundPlay(L"intro.mp3", 1);
	TitleSound->Position(120);
}
