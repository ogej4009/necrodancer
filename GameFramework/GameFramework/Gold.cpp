//#include "stdafx.h"
#include "Gold.h"
#include "GameState.h"
#include "Player.h"



Gold::Gold(CVector::MapIndex _MapIndex, int _Gold) : m_Len(0), m_GoldCount(0)
{
	m_ActorTypeIndex = EActorTypeIndex::InteractObject;
	m_Gold = CreateRender<GameRender>((int)m_ActorTypeIndex);
	m_Gold->SubSize(m_TileSize_Vector);
	m_Gold->SubPos({ _MapIndex.X * m_TileSize, _MapIndex.Y * m_TileSize });
	m_MapIndex = _MapIndex;
	m_GoldCount = _Gold;
	ChangeImg(_Gold);
}


Gold::~Gold()
{
}

void Gold::Update()
{
	if (State()->GetPlayer()->MapIndex() == m_MapIndex)
	{
		State()->GetPlayer()->ChangeGold(m_GoldCount);
		Death();
	}


	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 9)
		{
			m_Gold->Index(1);
		}
		else
		{
			m_Gold->Index(0);
		}
	}
}


void Gold::ChangeImg(int _Gold)
{
	switch (_Gold + 1)
	{
	case 0:
		m_Gold->Image(L"resource_coin1.bmp");
		break;
	case 1:
		m_Gold->Image(L"resource_coin2.bmp");
		break;
	case 2:
		m_Gold->Image(L"resource_coin3.bmp");
		break;
	case 3:
		m_Gold->Image(L"resource_coin4.bmp");
		break;
	case 4:
		m_Gold->Image(L"resource_coin5.bmp");
		break;
	case 5:
		m_Gold->Image(L"resource_coin6.bmp");
		break;
	case 6:
		m_Gold->Image(L"resource_coin7.bmp");
		break;
	case 7:
		m_Gold->Image(L"resource_coin8.bmp");
		break;
	case 8:
		m_Gold->Image(L"resource_coin9.bmp");
		break;
	case 9:
		m_Gold->Image(L"resource_coin10.bmp");
		break;
	default:
		m_Gold->Image(L"resource_coin10.bmp");
		break;
	}
}
