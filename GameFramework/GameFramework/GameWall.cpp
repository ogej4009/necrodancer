//#include "stdafx.h"
#include "GameWall.h"
#include "GameRender.h"
#include <GameTexMgr.h>
#include <GameMacro.h>
#include "GameState.h"


GameWall::GameWall(CVector _TargetPos, std::wstring _WallName) 
	: m_WallIndex(0)
	, m_Wall(nullptr)
	, m_Health(1)
	, m_Def(0)
{
	Pos({0,0});
	
	if (L"wall_stone_crypt.bmp" == _WallName)
	{
		m_Def = 2;
	}
	else if (L"wall_shop_crypt.bmp" == _WallName)
	{
		m_Def = 3;
	}
	else if (L"TEMP_wall_unbreakable.bmp" == _WallName)
	{
		m_Def = 10;
	}
	else if (L"boss_wall.bmp" == _WallName)
	{
		m_Def = 10;
	}
	else
	{
		m_Def = 1;
	}

	m_ActorTypeIndex = EActorTypeIndex::Wall;
	MapIndex(_TargetPos);
	m_CurPos = _TargetPos;
	m_RealPos = ActorCalcPos(_TargetPos);

	GameImage* tempImage = GameTexMgr::Inst().FindImg(_WallName);
	m_CutCount = static_cast<int>(tempImage->CutCount());

	m_Wall = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::Wall));

	m_Wall->SubPos(_TargetPos);
	_TargetPos.Y -= m_TileSizeHalf;
	m_Wall->SubPosPure(_TargetPos);
	m_Wall->SubSize({ m_TileSize, m_TileSize });
	m_Wall->Image(_WallName.c_str());
	m_Wall->Index(m_WallIndex);
	m_Wall->ShadowImage(L"WallShadow.bmp");
	m_Wall->ShadowIndex(0);
	

	State()->m_WallActorMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}

void GameWall::ChangeIndex(int _Index)
{
	if (_Index > m_CutCount /2)
	{
		ASSERTMSG;
		return;
	}
	m_Wall->Index(_Index);
}

GameWall::~GameWall()
{
}


void GameWall::Update()
{

}

void GameWall::DebugRender()
{
	m_RealPos = ActorCalcPos(m_CurPos);
	swprintf_s(m_DebugText, L"(%d,%d)",m_MapIndex.X , m_MapIndex.Y);
	TextOut(GameTexMgr::BackDC(), static_cast<int>(m_RealPos.X) , static_cast<int>(m_RealPos.Y), m_DebugText, lstrlen(m_DebugText));
}