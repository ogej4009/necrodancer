#pragma once
#include "Monster.h"


class CoralHead :  public Monster
{
private:
	friend MapEditor;

	AniRender* m_CoralHead;
	std::map<int, CVector::MapIndex> nextMap;
	CVector::MapIndex* ptrNextIndex;
	float m_DelayTime;
	bool m_CanAttack;
	bool m_Direction;
	int m_AttackCount;
	int m_SplashCount;
	int m_CurHealth;
	CVector m_InitPos;

	std::wstring CoralDeath;
	std::wstring CoralCry;
	std::wstring CoralHit[4];
	std::wstring CoralAttack[3];
	std::wstring CoralSplash[4];

public:
	static float GlobalTime;


public:
	CoralHead(CVector _TargetPos);
	~CoralHead();

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;

	void PlaySound(MonsterSound _SoundType) override;

	void InitSound();

	void Move();
	void ChangePos(CVector _Pos);

public:

	void Update() override;

};

