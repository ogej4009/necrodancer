//#include "stdafx.h"
#include "CenterHeart.h"
#include <GameWin.h>


CenterHeart::CenterHeart()
{
	m_Position.X = GameWin::MainWindow()->Size().X / 2;
	m_Position.Y = GameWin::MainWindow()->Size().Y - m_TileSize * 2;
	m_ActorTypeIndex = EActorTypeIndex::UI;

	InitRender(m_Position);
}


CenterHeart::~CenterHeart()
{
}



void CenterHeart::Update()
{

}

void CenterHeart::InitRender(CVector _Pos)
{
	AniRender* NewRender = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	NewRender->CreateAni(IDLE, L"TEMP_beat_heart.bmp", 0, 1, true, 0.3f);
	NewRender->SubPos(_Pos);
	NewRender->SubSize({ m_TileSize * 2 ,m_TileSize * 2 + m_TileSizeHalf});
	NewRender->ChangeAni(IDLE);
}