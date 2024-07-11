#pragma once
#include <list>
#include <map>
#include <GameMath.h>
#include <GameMacro.h>
#include "GameRender.h"
#include "AniRender.h"
#include "TileRender.h"
#include "GameCol.h"
#include "BaseUpdater.h"
#include "GameState.h"


enum class EActorTypeIndex
{
	Background = 0,
	Tile = 1,
	WaterTile,

	Text = 3,
	ShopFloor,

	BackWall = 5,


	Stair = 10,
	StairLock,
	InteractObject = 20,

	Weapon = 21,
	Armor,
	Helmet,
	ShopItem,
	Chest,


	PlayerLow = 25,
	PlayerLowBody,
	PlayerLowHead,


	Monster = 30,
	M_greenSlime,
	M_blueSlime,
	M_bat,
	M_skeleton,
	M_monkey,
	M_zombie,
	

	Player = 50,
	PlayerBody,
	PlayerHead,

	

	Door = 59,
	Wall = 60,

	M_minotaur,
	M_dragon,
	
	

	PlayerHigh = 65,
	PlayerHighBody,
	PlayerHighHead,

	EnemySwipe,

	ShopKeeper,

	Walltorch = 70,

	M_CoralDrum,
	M_CoralHorn,
	M_CoralKeytar,
	M_CoralString,
	M_CoralHead,

	UIBeatBar = 79,
	UI = 80,
	UIText,
	Max,
};



//각 장면에등장하는 요소들의 부모클래스
//시작버튼, 주인공 기타 등등을 전부 이 클래스의 포인터로 업캐스팅해서 다룬다.
class MapEditor;
class GameState;
class GameActor : public BaseUpdater
{

protected:
	enum {
		ArrSize = 40,
	};
	const float m_TilePadding;
	static const float m_TileSize;
	static const float m_TileSizeHalf;
	static const CVector m_TileSize_Vector;

public:
	static float TileSize()
	{
		return m_TileSize;
	}
	static float TileSizeHalf()
	{
		return m_TileSizeHalf;
	}
public:
	static std::map<__int64, GameActor*>::iterator FindIter;
	static std::map<__int64, GameActor*>::iterator EndIter;
	//static std::map<__int64, GameActor*> m_WallActorMap;
	//static std::map<__int64, GameActor*> m_InterObjMap;

public:
	EActorTypeIndex CheckType(EActorTypeIndex _Type)
	{
		EActorTypeIndex retType = m_ActorTypeIndex;
		switch (_Type)
		{
		case EActorTypeIndex::Monster:
		case EActorTypeIndex::M_greenSlime:
		case EActorTypeIndex::M_blueSlime:
		case EActorTypeIndex::M_bat:
		case EActorTypeIndex::M_skeleton:
		case EActorTypeIndex::M_minotaur:
		case EActorTypeIndex::M_monkey:
		case EActorTypeIndex::M_dragon:
			retType = EActorTypeIndex::Monster;
			break;
		case EActorTypeIndex::Player:
		case EActorTypeIndex::PlayerBody:
		case EActorTypeIndex::PlayerHead:
			retType = EActorTypeIndex::Player;
			break;
		default:
			break;
		}

		return retType;
	}

private:
	friend GameState;
	friend MapEditor;

	//자신을 그려줄 Scene에 대한 정보를 보관하는 곳
	GameState* m_State;

public:
	GameState* State()
	{
		return m_State;
	}

public:
	CVector ActorCalcPos(CVector _InitPos);

private:
	bool m_bUpdate; //enable, disable
	bool m_bDeath; //파괴, 이걸  Release함수에서 프레임마다 확인해서 지운다.

protected:
	EActorTypeIndex m_ActorTypeIndex;
	CVector::MapIndex m_MapIndex;

public:
	EActorTypeIndex ActorType() {
		return m_ActorTypeIndex;
	}

	void ActorType(EActorTypeIndex _Type) {
		m_ActorTypeIndex = _Type;
	}

	CVector::MapIndex MapIndex()
	{
		return m_MapIndex;
	}

	void MapIndex(CVector _Pos)
	{
		m_MapIndex = CalcIndex(_Pos);
		/*m_MapIndex.X = static_cast<int>(std::roundf(_Pos.X / m_TileSize));
		m_MapIndex.Y = static_cast<int>(std::roundf(_Pos.Y / m_TileSize));*/
	}

	CVector::MapIndex CalcIndex(CVector _Pos)
	{
		CVector::MapIndex NewIndex;
		NewIndex.X = static_cast<int>(roundf(_Pos.X / m_TileSize));
		NewIndex.Y = static_cast<int>(roundf(_Pos.Y / m_TileSize));
		return NewIndex;
	}


public:
	void ChangeActorType(EActorTypeIndex _Type)
	{
		m_ActorTypeIndex = _Type;
	}


public:
	void Death();
	bool IsDeath()
	{
		return m_bDeath;
	}

	EActorTypeIndex GetActorType()
	{
		return m_ActorTypeIndex;
	}

private:
	GameRect m_Rect; //지금은 일단 사각형으로 표현함

public:
	void Move(const CVector& _Pos)
	{
		m_Rect.Pos += _Pos;
	}

	void Pos(const CVector& _Pos) { m_Rect.Pos = _Pos; }
	void Size(const CVector& _Size) { m_Rect.Size = _Size; }
	// 4바이트 아낄려고 이러지는 않겠다.
	//const HVEC2& CPos() { return m_Pos; }
	//const HVEC2& CSize() { return m_Size; }
	CVector Pos() { return m_Rect.Pos; }
	CVector Size() { return m_Rect.Size; }

private:
	std::list<GameRender*>::iterator RenderStartIter;
	std::list<GameRender*>::iterator RenderEndIter;
	//Actor가 가지고 있을 자식 Render들 목록
	std::list<GameRender*> m_AllRenderList;

private:
	//Renderer의 생성과 삭제는 Actor내부에서 해야된다.
	//State는 랜더링 용으로 정보만 참조할 뿐 거기선 관리 자료구조 clear()만 해준다.
	void Release();

private:
	void AddRenderer(GameRender* _Render);


public:
	template<typename T>
	T* CreateRender(int _Order /*= 0*/)
	{
		T* NewRender = new T();
		NewRender->m_pParent = this;
		NewRender->m_Order = _Order;
		AddRenderer(NewRender);
		m_AllRenderList.push_back(NewRender);

		return NewRender;
	}

	/*GameRender* CreateRenderer(int _Order = 0);
	AniRender* CreateAniRenderer(int _Order = 0);*/

public:
	virtual void Update(); //Scene에선 액터마다 가지고 있는 이 Update를 돌려줌
	virtual void DebugRender(); //디버그 정보 출력용 렌더


private:
	std::list<GameCol*>::iterator ColStartIter;
	std::list<GameCol*>::iterator ColEndIter;
	std::list<GameCol*> m_AllColList;

public:
	// 콜백.
	// 내가 할일을 남한테 맡기는 것. 
	// 그 상황을 체크할수 있는것은???? 누구??? 충돌체
	// 그때 실행야할 함수를 구현해야 하는 것은 누구지?
	// 액터 상속받은 플레이어나 몬스터가 구현해야 한다.
	// 충돌체는 충돌여부, 충돌중인지 여부, 충돌 종료여부를 리턴해주고
	// 그 정보를 기반으로 액터들 내부에 구현한 함수를 동작시킨다.
	virtual void ColStay(GameCol* _ThisCol, GameCol* _OtherCol) {}
	virtual void ColEnter(GameCol* _ThisCol, GameCol* _OtherCol) {}
	virtual void ColExit(GameCol* _ThisCol, GameCol* _OtherCol) {}

public:
	//GameCol* CreateCol(GameCol::COLTYPE _Type, int _League);


	//Astar용 테스트 임시함수
	CVector CalPos();

public:
	GameActor();
	virtual ~GameActor();
};

