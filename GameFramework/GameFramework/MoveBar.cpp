//#include "stdafx.h"
#include "MoveBar.h"
#include <GameWin.h>
#include <GameTime.h>

MoveBar::MoveBar(bool _IsRight)
	: m_Position({ 0.0f, 0.0f }),
	m_HeartPosition({ 0,0 }),
	m_bIsRight(false),
	m_MoveRange(0),
	m_Timer(0)
{
	if (_IsRight)
	{
		m_bIsRight = _IsRight;
		m_Position.X = GameWin::MainWindow()->Size().X;
		m_Position.Y = GameWin::MainWindow()->Size().Y - m_TileSize * 2;
		m_ActorTypeIndex = EActorTypeIndex::UIBeatBar;
		m_HeartPosition.X = GameWin::MainWindow()->Size().X / 2;
		m_HeartPosition.Y = GameWin::MainWindow()->Size().Y - m_TileSize * 2;

		m_MarkerPos[0] = (int)m_Position.X;
		m_MarkerPos[1] = (int)m_Position.X - 120;
		m_MarkerPos[2] = (int)m_Position.X - 240;
		m_MarkerPos[3] = (int)m_Position.X - 360;
	}
	else
	{
		m_bIsRight = _IsRight;
		m_Position.X = 0;
		m_Position.Y = GameWin::MainWindow()->Size().Y - m_TileSize * 2;
		m_ActorTypeIndex = EActorTypeIndex::UIBeatBar;
		m_HeartPosition.X = GameWin::MainWindow()->Size().X / 2;
		m_HeartPosition.Y = GameWin::MainWindow()->Size().Y - m_TileSize * 2;

		m_MarkerPos[0] = (int)m_Position.X;
		m_MarkerPos[1] = (int)m_Position.X + 120;
		m_MarkerPos[2] = (int)m_Position.X + 240;
		m_MarkerPos[3] = (int)m_Position.X + 360;
	}



	for (int i = 0; i < 4; ++i)
	{
		InitRender(m_Position, i);
	}

}


MoveBar::~MoveBar()
{
}


void MoveBar::barMove()
{
	if (m_bIsRight)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_MoveBar[i]->SubPos(CVector::Lerp(CurPos[i], NextPos[i], m_Timer * 3));
			if (m_MoveBar[i]->CalPos().X <= 480)
			{
				m_MoveBar[i]->SubPos(m_Position);
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			m_MoveBar[i]->SubPos(CVector::Lerp(CurPos[i], NextPos[i], m_Timer*3));
			if (m_MoveBar[i]->CalPos().X >= 480)
			{
				m_MoveBar[i]->SubPos(m_Position);
			}
		}
	}
	
}


void MoveBar::Update()
{

	m_Timer += GameTime::DeltaTime();
	barMove();

	if (m_Timer - 0.34f < 0)
	{
		return;
	}

	m_Timer = 0;
	if (m_bIsRight)
	{
		for (int i = 0; i < 4; ++i)
		{
			CurPos[i] = m_MoveBar[i]->CalPos();
			NextPos[i] = CurPos[i] - CVector::RIGHT * 180;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			CurPos[i] = m_MoveBar[i]->CalPos();
			NextPos[i] = CurPos[i] + CVector::RIGHT * 180;
		}
	}


	
}

void MoveBar::InitRender(CVector _Pos, int _Index)
{
	_Pos.X = (float)m_MarkerPos[_Index];
	m_MoveBar[_Index] = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
	m_MoveBar[_Index]->TurnOffCam();
	m_MoveBar[_Index]->SubPos(_Pos);
	CurPos[_Index] = _Pos;
	NextPos[_Index] = _Pos;
	m_MoveBar[_Index]->SubSize({ 10.0f , 45.0f });
	m_MoveBar[_Index]->Image(L"TEMP_beat_marker.bmp");
	m_MoveBar[_Index]->Index(0);
	m_MoveBar[_Index]->ShadowImage(L"TEMP_beat_markerShadow.bmp");
	m_MoveBar[_Index]->ShadowIndex(0);

}
