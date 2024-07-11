#pragma once
#include "GameActor.h"


//타일 찍어주는 클래스
class TileRender;
class GameMap : public GameActor
{
private:
	static TileRender* m_TileRen;

public:
	GameMap(EActorTypeIndex _Type = EActorTypeIndex::Tile);
	~GameMap();

public:
	static TileRender* TileRen()
	{
		return m_TileRen;
	}
};

