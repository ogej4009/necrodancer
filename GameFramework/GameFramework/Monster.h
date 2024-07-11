#pragma once
#include <list>
#include <GameTime.h>
#include <GameSound.h>
#include "GameState.h"
#include "GameActor.h"
#include "Player.h"



class GameCol;
class Monster :
	public GameActor
{
protected:
	std::vector<GameRender*> m_AllHeart;
	CVector::MapIndex m_MonsterPosIndex;
	CVector::MapIndex m_MonsterFourPos[4];
	float m_Health;
	int m_Gold;
	CVector m_DefaultSize;
	CVector m_CurPos;
	CVector m_NextPos;
	CVector m_CalPos;
	CVector m_StartPos;
	CVector m_EndPos;
	wchar_t m_DebugText[256];
	int m_Len;
	int m_LenArr[4];
	int m_HitCount;
	bool m_bHeartMake;
	bool m_firstContact;
	float m_MoveDelayTime;
	float m_AttackTime;
	CVector::MapIndex prevIndex;
	CVector::MapIndex nextIndex;

	std::map<__int64, GameActor*>::iterator findIter;
	std::map<__int64, GameActor*>::iterator endIter;
	std::map<__int64, GameActor*>::iterator m_WallIter;
	std::map<__int64, GameActor*>::iterator m_WallEndIter;


	AniRender* DustRenderer;

	//충돌용도가 아니고 이미지 디버깅용도로 사용함
	std::list<GameCol*> m_ColList;
	std::list<GameCol*>::iterator colStartIter;
	std::list<GameCol*>::iterator colEndIter;

	enum MonsterDir
	{
		Left,
		Up,
		Right,
		Down
	};

	enum MonsterSound {
		Attack,
		Hit,
		MonsterDeath,
		Walk,
		Cry,
		Charge,
		WallCrush,
		Grab,
		Headloss
	};

	enum MonsterState {
		IDLE,
		Activity,
		MinoDown,
		ShadowIDLE,
		ShadowActivity,
		SkelLoss,
	};

	MonsterState m_EState;

public:
	float m_MoveTime;

public:
	Monster();
	virtual ~Monster();

public:
	CVector::MapIndex MonsterPosIndex()
	{
		return m_MonsterPosIndex;
	}
	bool Damaged(int _Attack);

public:
	std::map<__int64, GameActor*> MonsterMap()
	{
		return State()->m_MonsterMap;
	}

protected:
	virtual void InitFSM();
	virtual void InitDebug(CVector _TargetPos);
	virtual void InitAni(CVector _TargetPos);
	virtual void PlaySound(MonsterSound _SoundType);
	virtual void ChangeState(MonsterState _State);
	void DebugRender() override;
	void MonsterMove(CVector _CurPos, CVector _NextPos);
	void InitHeart(CVector _TargetPos, int _Attack);
	void ClearHeart();

private:
	void InitDirtParticle();
	void Update() override;
};

