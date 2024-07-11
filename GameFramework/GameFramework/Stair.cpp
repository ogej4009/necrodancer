//#include "stdafx.h"
#include "Stair.h"

Stair::Stair(CVector _TargetPos)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::Stair;
	CVector tempPos = _TargetPos;

	m_Stair = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::Stair));
	m_Stair->SubPos(_TargetPos);
	m_Stair->SubSize({ m_TileSize, m_TileSize });
	m_Stair->Image(L"stairs.bmp");

	MapIndex(_TargetPos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}


Stair::~Stair()
{
}


void Stair::Update()
{
}