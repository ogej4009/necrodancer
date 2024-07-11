//#include "stdafx.h"
#include "Heart.h"
#include "GameState.h"
#include "Player.h"
#include <GameWin.h>
#include <GameTime.h>

//맵 로딩하는 부분 위쪽에 무조건 생성하게 하자.
//업데이트를 돌면서 플레이어 hp를 체크하게 하고.
Heart::Heart(int _HeartCount)
	: m_Position({ 0.0f, 0.0f })
	, m_HeartCount(0)
	, m_HeartGroupCount(0)
	, m_Timer(0.0f)
	, m_BeatCount(0)
{
	m_Position.X = GameWin::MainWindow()->Size().X - (m_TileSize * 7.5f);
	m_Position.Y = m_TileSizeHalf + m_TileSizeHalf / 2;

	m_HeartCount = _HeartCount;
	m_HeartGroupCount = _HeartCount >> 1;
	m_ActorTypeIndex = EActorTypeIndex::UI;
	InitAni(m_Position);
}


Heart::~Heart()
{
}


void Heart::InitAni(CVector _TargetPos)
{
	AniRender* NewHeart = nullptr;
	for (int i = 0; i < m_HeartGroupCount; ++i)
	{
		NewHeart = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
		NewHeart->SubPos(_TargetPos);
		_TargetPos.X += m_TileSize + 3.0f;
		NewHeart->SubSize({ m_TileSize ,m_TileSize });
		NewHeart->CreateAni(FULL, L"heart_ani.bmp", 0, 1, false, 0.125f);
		NewHeart->CreateAni(HALF, L"heart_half.bmp", 0, 0, true, 0.5f);
		NewHeart->CreateAni(NONE, L"heart_empty.bmp", 0, 0, true, 0.5f);
		NewHeart->ChangeAni(FULL);
		m_Heart.push_back(NewHeart);
	}
}


void Heart::Update()
{
	m_HeartCount = State()->GetPlayer()->Health();
	CheckHeart();
	HeartBeat();
}

void Heart::HeartBeat()
{
	m_Timer += GameTime::DeltaTime();

	if (0.125f < m_Timer)
	{
		for (int i = 0; i < 5; ++i)
		{
			m_Heart[i]->ChangeLoopOff(FULL);
		}
	}

	if (0.6f < m_Timer)
	{
		m_Timer = 0.0f;
		++m_BeatCount;

		if (m_BeatCount == 5)
		{
			m_BeatCount = 0;
		}

		switch (m_BeatCount)
		{
		case 0:
			m_Heart[4]->ChangeLoopOff(FULL);
			m_Heart[0]->ChangeLoopOn(FULL);
			break;
		case 1:
			m_Heart[0]->ChangeLoopOff(FULL);
			m_Heart[1]->ChangeLoopOn(FULL);
			break;
		case 2:
			m_Heart[1]->ChangeLoopOff(FULL);
			m_Heart[2]->ChangeLoopOn(FULL);
			break;
		case 3:
			m_Heart[2]->ChangeLoopOff(FULL);
			m_Heart[3]->ChangeLoopOn(FULL);
			break;
		case 4:
			m_Heart[3]->ChangeLoopOff(FULL);
			m_Heart[4]->ChangeLoopOn(FULL);
			break;
		default:
			ASSERTMSG;
			break;
		}

	}

	
	
}

void Heart::CheckHeart()
{
	switch (m_HeartCount)
	{
	case 10:
	{
		break;
	}
	case 9:
	{
		m_Heart[4]->ChangeAni(HALF);
		break;
	}
	case 8:
	{
		m_Heart[4]->ChangeAni(NONE);
		break;
	}
	case 7:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(HALF);
		break;
	}
	case 6:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		break;
	}
	case 5:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(HALF);
		break;
	}
	case 4:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(NONE);
		break;
	}
	case 3:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(NONE);
		m_Heart[1]->ChangeAni(HALF);
		break;
	}
	case 2:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(NONE);
		m_Heart[1]->ChangeAni(NONE);
		break;
	}
	case 1:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(NONE);
		m_Heart[1]->ChangeAni(NONE);
		m_Heart[0]->ChangeAni(HALF);
		break;
	}
	case 0:
	{
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(NONE);
		m_Heart[1]->ChangeAni(NONE);
		m_Heart[0]->ChangeAni(NONE);
		break;
	}

	default:
		m_Heart[4]->ChangeAni(NONE);
		m_Heart[3]->ChangeAni(NONE);
		m_Heart[2]->ChangeAni(NONE);
		m_Heart[1]->ChangeAni(NONE);
		m_Heart[0]->ChangeAni(NONE);
		break;
	}
}
