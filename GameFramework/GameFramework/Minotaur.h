#pragma once
#include "Monster.h"


class MapEditor;
class Minotaur : public Monster
{

private:
	AniRender* m_Minotaur;
	CVector::MapIndex* ptrNextIndex;
	std::map<int, CVector::MapIndex> nextMap;
	float m_DelayTime;
	bool m_bCharge;
	bool m_Direction;
	bool m_Delay;
	int m_HitCount;
	float m_AttackSpeed;
	float m_AttackDelay;
	bool m_AttackCheck;

	std::wstring MinotaurHit[3];
	std::wstring MinotaurAttack;
	std::wstring MinotaurDeath;
	std::wstring MinotaurCharge;
	std::wstring MinotaurCry;
	std::wstring MinotaurWallImpact;

public:
	Minotaur(CVector _TargetPos);
	~Minotaur();

public:
	friend MapEditor;
	void Update() override;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void InitSound();
	void ChangeState(MonsterState _State) override;
	void PlaySound(MonsterSound _SoundType) override;

	void Move();
	void StopMino();
	void MoveFaster();
	void CheckAttack(float _AttackSPD);
	void ChangePos(CVector _Pos);
};

