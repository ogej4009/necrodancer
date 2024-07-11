//#include "stdafx.h"
#include "GameMap.h"
#include "TileRender.h"


TileRender* GameMap::m_TileRen = nullptr;

GameMap::GameMap(EActorTypeIndex _Type)
{
	m_ActorTypeIndex = _Type;
	m_TileRen = CreateRender<TileRender>(static_cast<int>(_Type));
	m_TileRen->ShadowImage(L"TILESH.bmp");
	m_TileRen->ShadowIndex(0);

}

GameMap::~GameMap()
{
}
