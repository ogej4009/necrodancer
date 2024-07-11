//#include "stdafx.h"
#include "ShopFloor.h"


ShopFloor::ShopFloor(CVector _TargetPos)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::ShopFloor;
	CVector tempPos = _TargetPos;

	m_ShopFloor = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::ShopFloor));
	m_ShopFloor->SubPos(_TargetPos);
	m_ShopFloor->SubSize({ m_TileSize, m_TileSize });
	m_ShopFloor->Image(L"shop_floor.bmp");
	m_ShopFloor->ShadowImage(L"TILESH.bmp");
	m_ShopFloor->ShadowIndex(0);

	MapIndex(_TargetPos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}


ShopFloor::~ShopFloor()
{
}

void ShopFloor::Update()
{
	
}