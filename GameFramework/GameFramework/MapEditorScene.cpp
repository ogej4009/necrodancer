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
	// �� ���� ��ҵ��� ����� �Ұ��̴�.
	// CreateActor()�� �ҷ��� �ߴµ� 
	// �ϴ� Actor�� �׳� ����� ���·� �ؾ��ϴµ�
	// �����ڿ��� �ڵ����� �����ǰ� �Ǵ� ���·� ���������.
	//Player* NewPlayer = new Player();
	//TILE* NewTile = new TILE();
	//MapEditor* NewTileMap = new MapEditor(L"Test.DATA");
	//GameWall* NewWall = new GameWall();
}


void MapEditorScene::StateUpdate()
{
	//GameState::ChangeState(L"PLAY");
}
