#pragma once
#include "Monster.h"


class MapEditor;
class Dragon : public Monster
{

private:
	AniRender* m_Dragon;

public:
	Dragon(CVector _TargetPos);
	~Dragon();

private:
	friend MapEditor;
	void Update() override;
	int m_HitCount;
	int m_WalkCount;
	int m_LenArr[4];
	float m_AttackSpeed;
	float m_AttackDelay;
	bool m_AttackCheck;
	bool m_ColCheck;
	std::map<int, CVector::MapIndex> nextMap;

	std::wstring DragonHit[4];
	std::wstring DragonWalk[2];
	std::wstring DragonDeath;
	std::wstring DragonAttack;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;

	void InitSound();
	void PlaySound(MonsterSound _SoundType) override;

	void Move();
	void ChangePos(CVector _Pos);

};

