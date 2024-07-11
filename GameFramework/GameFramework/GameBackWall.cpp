//#include "stdafx.h"
#include "GameBackWall.h"
#include <GameTexMgr.h>
#include <GameMacro.h>
#include "GameRender.h"

GameBackWall::GameBackWall(CVector _TargetPos, std::wstring _WallName)
	: m_BackWallIndex(0)
	, m_BackWall(nullptr)
{
	Pos({ 0,0});
	m_ActorTypeIndex = EActorTypeIndex::BackWall;
	MapIndex(_TargetPos);

	GameImage* tempImage = GameTexMgr::Inst().FindImg(_WallName);
	m_CutCount = static_cast<int>(tempImage->CutCount());

	m_BackWallIndex = m_CutCount / 2;
	m_BackWall = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::BackWall));
	
	m_BackWall->SubPos(_TargetPos);
	_TargetPos.Y += m_TileSize / 2;
	m_BackWall->SubPosPure(_TargetPos);
	m_BackWall->SubSize({ m_TileSize, m_TileSize });
	m_BackWall->Image(_WallName.c_str());
	m_BackWall->Index(m_BackWallIndex);
	m_BackWall->ShadowImage(L"WallShadow.bmp");
	m_BackWall->ShadowIndex(1);

	State()->m_BackWallActorMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}

void GameBackWall::Update()
{

}


void GameBackWall::ChangeIndex(int _Index)
{
	if (_Index > m_CutCount)
	{
		ASSERTMSG;
		return;
	}
	m_BackWall->Index(_Index);
}


GameBackWall::~GameBackWall()
{
}
