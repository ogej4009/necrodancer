#pragma once
#include <list>
#include "GameActor.h"
#include <GameSound.h>
#include "ShopItem.h"


class Monkey;
class AniRender;
class Player : public GameActor
{
private:

	//외부 공개시 enum class로 바꿀것
	enum PLAYERSTATE
	{
		IDLE,
		MOVE,

		BUY_ON_MOVE,
		ITEM_ON_MOVE,
		GOLD_ON_MOVE,
		DOOR_ON_MOVE,

		WALL_MOVE,
		WALL_ATTACK,
		WALL_DIAMOND_MOVE,
		WALL_DIAMOND_ATTACK,
		WALL_ITEM_MOVE,
		WALL_ITEM_ATTACK,

		MONSTER_MOVE,
		MONSTER_ATTACK,

		WET_FLOOR_MOVE,
		THROW_ITEM,

		WALL_LOCK_MOVE,
		DOOR_LOCK_MOVE,

		OnArmor,

		STATE_MAX,
	};

	enum eWeapon {
		Dagger,
		Rapier,
		LongSword,
		BroadSword,
	};

	enum  WeponDir
	{
		Dagger_Left,
		Dagger_Up,
		Dagger_Right,
		Dagger_Down,

		Rapier_Left,
		Rapier_Up,
		Rapier_Right,
		Rapier_Down,

		LongSword_Left,
		LongSword_Up,
		LongSword_Right,
		LongSword_Down,

		BroadSword_Left,
		BroadSword_Up,
		BroadSword_Right,
		BroadSword_Down
	};

public:
	enum class InterActItem
	{
		Weapon,
		Gold,
		BodyArmor,
		Helmet,
		Magic,
		Bomb,
	};


public:
	static CVector::MapIndex PlayerIndex;
	static CVector::MapIndex nextPlayerIndex;

private:
	static int m_StaticPlayerWeaponType;
	static int m_StaticPlayerHealth;
	static int m_StaticPlayerGold;
	static bool m_StaticHead;
	static bool m_StaticArmor;
	


private:

	PLAYERSTATE m_EState;
	Monkey* m_ptrMonkey;
	int m_PlayerHealth;
	int m_PlayerGold;
	int m_PlayerDiamond;
	int m_PlayerShovelLev;

	AniRender* m_PlayerLowHead;
	AniRender* m_PlayerLowBody;
	AniRender* m_PlayerHead;
	AniRender* m_PlayerBody;
	AniRender* m_PlayerHighHead;
	AniRender* m_PlayerHighBody;
	AniRender* m_Shovel;
	GameRender* m_Damaged;


	eDir m_Dir;
	eDir m_PrevDir;
	bool m_bKeyInput;
	bool m_bReadyInput;
	bool m_bCheckWall;
	bool m_bCheckMonster;
	bool m_bWaterTileCheck;
	bool m_bCanMove;
	bool m_bIsMove;
	bool m_bIsJump;
	bool m_bOnStair;
	bool m_bOpenChest;
	static float m_JumpTimer;
	static float m_FPSUpdate;
	int m_FPS;
	CVector m_CurPos;
	CVector m_NextPos;
	CVector::MapIndex m_PrevIndex;
	//CVector m_MonsterPos;
	CVector::MapIndex MonsterIndex;
	CVector::MapIndex m_PlayerIndexArr[25];
	int Xindex;
	int Yindex;
	int m_CurXIndex;
	int m_CurYIndex;
	int MouseX;
	int MouseY;
	int m_AttackCount;
	int m_WaterJump;
	float m_DamagedTimer;

	bool m_WaterSound;
	bool m_bOnItem;
	InterActItem m_ItemType;

	//Astar 테스트용 멤버
	std::list<CVector> PathList;
	float MoveTime;

	//오디오 멤버
	std::wstring PlayerStateSound[2];
	std::wstring PlayerMeleeSound[15];
	std::wstring PlayerDigSound[4];
	std::wstring PlayerWaterInOut[2];
	static int m_SoundIndex;

	//공격판정 받은 몬스터 넣어놓는 배열
	GameActor* HitMonsterArr[3];
	//Swipe
	AniRender* PlayerSwipe[4];
	AniRender* EnemySwipe;
	//외부에서 받은 무기 객체를 가리키는 포인터
	GameActor* m_ExtTItem;

	eWeapon m_WeaponType;
	WeponDir m_WeaponDir;

	bool m_Armor;
	bool m_Head;

	bool m_OnMonkey;

	void CheckWeponDir();
	void PlayWeaponAni();

public:
	Player();
	~Player();


public:
	void Update() override;
	void DebugRender() override;

	void Damaged(int atkDmg);

	void OnMonkey(Monkey* _Monkey)
	{
		m_ptrMonkey = _Monkey;
		m_OnMonkey = true;
	}

	void ChangeGold(unsigned int Gold)
	{
		m_PlayerGold += (int)Gold;
	}

	int PlayerGold()
	{
		return m_PlayerGold;
	}

	bool CanMove()
	{
		return m_bCanMove;
	}

	int Health()
	{
		return m_PlayerHealth;
	}

    void InitPlayerSound();

	bool OnStair()
	{
		return m_bOnStair;
	}

	int CurWeaponType()
	{
		return (int)m_WeaponType;
	}

	void ChangeWeaponType(int _Type)
	{
		m_WeaponType = (eWeapon)_Type;
	}

	bool CurArmor()
	{
		return m_Armor;
	}

	bool CurHead()
	{
		return m_Head;
	}

	//외부 아이템과 상호작용 하기위한 함수
	void OnItem(GameActor* _Item, InterActItem _ItemType);
	void OnHead()
	{
		m_Head = true;
	}
	void ChangeHealth()
	{
		m_PlayerHealth = 0;
	}
	bool OnBuying(int _price);
	void CheckItem();
	void ChangeGlobal();

private:

	
	void PlayerMove();

	void DirCheck();
	void DirChangeCheck();

	void InitFSM();
	void InitAni();

	void CheckIndex();
	void CheckCurIndex();

	void WallCheck();
	void WallOrderCheck();
	void StairCheck();

	void MonsterCheck();
	void MonsterOrderCheck();

private:
	void DirChangeState(PLAYERSTATE _Change);

public:
	bool m_bPressDown;
	bool m_bPressLeft;
	bool m_bPressRight;
	bool m_bPressUp;

	//FSM목록
private:
	void F_IDLE();
	void F_MOVE();
	void F_BUY_ON_MOVE();
	void F_ITEM_ON_MOVE(GameActor* _Item);
	void F_GOLD_ON_MOVE();
	void F_DOOR_ON_MOVE();
	void F_WALL_MOVE();
	void F_WALL_ATTACK();
	void F_WALL_DIAMOND_MOVE();
	void F_WALL_DIAMOND_ATTACK();
	void F_WALL_ITEM_MOVE();
	void F_WALL_ITEM_ATTACK();
	void F_MONSTER_MOVE();
	void F_MONSTER_ATTACK();
	void F_WET_FLOOR_MOVE();
	void F_THROW_ITEM();
	void F_WALL_LOCK_MOVE();
	void F_DOOR_LOCK_MOVE();
};

