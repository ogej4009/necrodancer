//#include "stdafx.h"
#include "GameDoor.h"
#include <GameTime.h>



GameDoor::GameDoor(CVector _TargetPos, bool _bIsSide) : m_bIsSide(false), m_DelayTime(0), m_bOpen(false)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::Door;
	MapIndex(_TargetPos);

	m_bIsSide = _bIsSide;
	m_Door = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::Door));

	if (m_bIsSide)
	{
		m_Door->SubPos(_TargetPos);
		m_Door->SubSize({ m_TileSize, m_TileSize * 2 });
		m_Door->Image(L"door_side.bmp");
	}
	else
	{
		m_Door->SubPos(_TargetPos);
		m_Door->SubSize({ m_TileSize, m_TileSize * 2 });
		m_Door->Image(L"door_front.bmp");
	}

	State()->m_WallActorMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));

}


void GameDoor::OpenDoor()
{
	m_bOpen = true;
	GameSound::SoundPlayToOne(L"obj_door_open.mp3");
}

GameDoor::~GameDoor()
{
}

void GameDoor::Update()
{
	if (m_bOpen)
	{
		m_DelayTime += GameTime::DeltaTime();
	}

	if (m_DelayTime > 0.2f)
	{
		auto tempIter = State()->m_WallActorMap.find(m_MapIndex.Index);
		State()->m_WallActorMap.erase(tempIter);
		Death();
	}


}
