#pragma once
#include "Monster.h"


class MapEditor;
class Bat :
	public Monster
{
private:
	AniRender* m_Bat;
	float m_Timer;

public:
	Bat(CVector _TargetPos);
	~Bat();

public:
	friend MapEditor;
	void Update() override;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void InitSound();
	void ChangeState(MonsterState _State) override;
	void PlaySound(MonsterSound _SoundType) override;

	std::wstring BatHit;
	std::wstring BatDeath;
	std::wstring BatAttack;

	void Move();
	void ChangePos(CVector _Pos);
};

