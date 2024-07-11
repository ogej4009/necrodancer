#pragma once
#include "Monster.h"


class MapEditor;
class Horn :
	public Monster
{
private:
	friend MapEditor;

	AniRender* m_CoralHorn;
	std::map<int, CVector::MapIndex> nextMap;
	CVector::MapIndex* ptrNextIndex;
	float m_DelayTime;
	bool m_CanAttack;
	bool m_Direction;
	static int NumberCount;
	std::wstring Numbering[2];

	int m_MyCount;
	float m_AttackDelay;
	bool m_Changed;
	CVector::MapIndex firstIndex;

public:
	Horn(CVector _TargetPos);
	~Horn();

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;

	void Move();
	void ChangePos(CVector _Pos);

public:

	void Update() override;
};

