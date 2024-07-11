#pragma once
#include "Monster.h"

class MapEditor;
class Skeleton : public Monster
{
private:
	AniRender* m_Skeleton;
	std::map<int, CVector::MapIndex> nextMap;
	CVector::MapIndex* ptrNextIndex;
	float m_DelayTime;
	bool m_CanAttack;
	bool m_Direction;

	std::wstring SkelDeath;
	std::wstring SkelHeadLoss;
	std::wstring SkelHit[2];
	std::wstring SkelAttack;



public:
	Skeleton(CVector _TargetPos);
	~Skeleton();

public:
	friend MapEditor;
	void Update() override;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;

	void PlaySound(MonsterSound _SoundType) override;

	void InitSound();

	void Move();
	void ChangePos(CVector _Pos);
};

