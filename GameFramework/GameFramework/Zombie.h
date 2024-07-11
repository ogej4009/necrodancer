#pragma once
#include "Monster.h"

class Zombie : public Monster
{
private:
	enum Quardant
	{
		Zero,
		One,
		Two,
		Three,
		Four,
		XPositive,
		YPositive,
		XNegative,
		YNegative,
		ShadowOne,
		ShadowThree,
		ShadowFour,

	};


private:
	AniRender* m_Zombie;
	std::map<int, CVector::MapIndex> nextMap;
	CVector::MapIndex* ptrNextIndex;
	CVector::MapIndex m_PlayerIndex;
	Quardant m_Quardant;
	Quardant m_PrevQuardant;
	float m_DelayTime;
	bool m_CanAttack;
	bool m_Direction;

	std::wstring ZombieDeath;
	std::wstring ZombieAttack;

public:
	Zombie(CVector _TargetPos);
	~Zombie();

public:
	friend MapEditor;
	void Update() override;

private:
	void SetQuardant();
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;

	void PlaySound(MonsterSound _SoundType) override;

	void InitSound();

	void Move();
void ChangePos(CVector _Pos); 

};

