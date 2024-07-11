#pragma once
#include <list>
#include <GameFile.h>
#include <GameDic.h>
#include "GameActor.h" 

class GameMap;
class GameFile;
class GameWall;
class GameDoor;
class Torch;
class GreenSlime;
class BlueSlime;
class Bat;
class Skeleton;
class Minotaur;
class Monkey;
class Dragon;
class Stair;
class Text;
class Monster;
class TileRender;
class MapEditor : public GameActor
{
	enum class ObjectData {
		tile = -1,
		wall = 0,
		door,
		doorSide,
		shopFloor,
		shopItem,
		stair,
		stairlock,
		walltorch,
		cryptWall,
		dirtWall,
		dirtWallCrack,
		dirtWallNestItem,
		dirtWallNestDia,
		stoneWall,
		stoneWallCrack,
		unbreakable,
		bossWall,
		waterTile,
		chest,
		chest2,
		weapon,
		armor,
		helmet,
		shopKeeper,
		greenSlime,
		blueSlime,
		bat,
		skeleton,
		zombie,
		minotaur,
		monkey,
		coralhead,
		coraldrum,
		coralhorn,
		coratkeytar,
		coralstring,
		dragon,
		text_HelloWorld,
	

		Max,
	};

public:
	//SGAMEACTOR에서 몬스터, 벽이 사라질때 여기서도 빼주기
	std::list<Monster*>& GetMonsterList()
	{
		return m_ListMonster;
	}

	std::list<GameActor*>& GetMapActor()
	{
		return m_ListMapActor;
	}


private:
	//벡터 원소 위치정보, 이름
	//MapActor로 잡다한거 관리하기
	//충돌체크는 어차피 Actor에 있는 맵으로 해야함
	//저장, 로딩용도로만 사용
	std::list<GameActor*> m_ListMapActor;
	std::list<Monster*> m_ListMonster;
	std::wstring m_WallName[8];
	std::wstring SaveFileName;

	bool m_WaterTileMake;
	CVector m_MousePos;
	CVector m_CalcMousePos;
	GameFile* m_SaveFile;
	GameDic m_GameDir;
	ObjectData m_EObjectdata;

	//디버깅용
	wchar_t buff[100];

	//타일용도
	//자체적으로 추가되는 타일을 관리하는 자료구조
	GameMap* m_TileMap;
	GameMap* m_WaterTileMap;
	TileRender* TileRenderer;
	TileRender* WaterTileRenderer;

	std::wstring m_TileName[2];
	int m_TileNameIndex;


public:
	MapEditor(std::wstring _SaveFile , int m_TileNameIndex = 0);
	~MapEditor();

public:
	void Update() override;
	void ChangeTileIndex(int _zeroOrOne)
	{
		m_TileNameIndex = _zeroOrOne;
	}


private:
	void TilemapInit();
	void CheckObjectType();
	//void InitUI();
};

