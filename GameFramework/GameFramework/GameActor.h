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



//�� ��鿡�����ϴ� ��ҵ��� �θ�Ŭ����
//���۹�ư, ���ΰ� ��Ÿ ����� ���� �� Ŭ������ �����ͷ� ��ĳ�����ؼ� �ٷ��.
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

	//�ڽ��� �׷��� Scene�� ���� ������ �����ϴ� ��
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
	bool m_bDeath; //�ı�, �̰�  Release�Լ����� �����Ӹ��� Ȯ���ؼ� �����.

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
	GameRect m_Rect; //������ �ϴ� �簢������ ǥ����

public:
	void Move(const CVector& _Pos)
	{
		m_Rect.Pos += _Pos;
	}

	void Pos(const CVector& _Pos) { m_Rect.Pos = _Pos; }
	void Size(const CVector& _Size) { m_Rect.Size = _Size; }
	// 4����Ʈ �Ƴ����� �̷����� �ʰڴ�.
	//const HVEC2& CPos() { return m_Pos; }
	//const HVEC2& CSize() { return m_Size; }
	CVector Pos() { return m_Rect.Pos; }
	CVector Size() { return m_Rect.Size; }

private:
	std::list<GameRender*>::iterator RenderStartIter;
	std::list<GameRender*>::iterator RenderEndIter;
	//Actor�� ������ ���� �ڽ� Render�� ���
	std::list<GameRender*> m_AllRenderList;

private:
	//Renderer�� ������ ������ Actor���ο��� �ؾߵȴ�.
	//State�� ������ ������ ������ ������ �� �ű⼱ ���� �ڷᱸ�� clear()�� ���ش�.
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
	virtual void Update(); //Scene���� ���͸��� ������ �ִ� �� Update�� ������
	virtual void DebugRender(); //����� ���� ��¿� ����


private:
	std::list<GameCol*>::iterator ColStartIter;
	std::list<GameCol*>::iterator ColEndIter;
	std::list<GameCol*> m_AllColList;

public:
	// �ݹ�.
	// ���� ������ ������ �ñ�� ��. 
	// �� ��Ȳ�� üũ�Ҽ� �ִ°���???? ����??? �浹ü
	// �׶� ������� �Լ��� �����ؾ� �ϴ� ���� ������?
	// ���� ��ӹ��� �÷��̾ ���Ͱ� �����ؾ� �Ѵ�.
	// �浹ü�� �浹����, �浹������ ����, �浹 ���Ῡ�θ� �������ְ�
	// �� ������ ������� ���͵� ���ο� ������ �Լ��� ���۽�Ų��.
	virtual void ColStay(GameCol* _ThisCol, GameCol* _OtherCol) {}
	virtual void ColEnter(GameCol* _ThisCol, GameCol* _OtherCol) {}
	virtual void ColExit(GameCol* _ThisCol, GameCol* _OtherCol) {}

public:
	//GameCol* CreateCol(GameCol::COLTYPE _Type, int _League);


	//Astar�� �׽�Ʈ �ӽ��Լ�
	CVector CalPos();

public:
	GameActor();
	virtual ~GameActor();
};

