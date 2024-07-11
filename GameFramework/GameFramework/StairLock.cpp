//#include "stdafx.h"
#include "StairLock.h"


StairLock::StairLock(CVector _TargetPos)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::StairLock;
	CVector tempPos = _TargetPos;

	m_Stair = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::StairLock));
	m_Stair->SubPos(_TargetPos);
	m_Stair->SubSize({ m_TileSize, m_TileSize });
	m_Stair->Image(L"stairs_locked_miniboss.bmp");

	MapIndex(_TargetPos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}


StairLock::~StairLock()
{
}


void StairLock::Update()
{
	if (State()->MiniDie())
	{
		m_Stair->Image(L"stairs.bmp");
	}
	else
	{
		m_Stair->Image(L"stairs_locked_miniboss.bmp");
	}
}
