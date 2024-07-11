//#include "stdafx.h"
#include "MapEditorScene.h"
#include <GameInput.h>
#include <GameWin.h>
#include "Player.h"
#include "MapEditor.h"
#include "GameWall.h"
#include "ValueData.h"
#include "GameMap.h"
#include "TileRender.h"


MapEditorScene::MapEditorScene()
{
}


MapEditorScene::~MapEditorScene()
{
}


void MapEditorScene::Loading()
{
	//Link(eColGroup::PLAYER, eColGroup::MONSTER);
	//Link(eColGroup::PLAYER, eColGroup::MONSTERBULLET);
	// 내 게임 요소들을 만들게 할것이다.
	// CreateActor()로 할려고 했는데 
	// 일단 Actor는 그냥 만드는 형태로 해야하는듯
	// 생성자에서 자동으로 관리되게 되는 형태로 만들어졌다.
	//Player* NewPlayer = new Player();
	//TILE* NewTile = new TILE();
	//MapEditor* NewTileMap = new MapEditor(L"Test.DATA");
	//GameWall* NewWall = new GameWall();
}


void MapEditorScene::StateUpdate()
{
	//GameState::ChangeState(L"PLAY");
}
