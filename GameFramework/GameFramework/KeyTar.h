#pragma once
#include "Monster.h"


class MapEditor;
class Keytar :
	public Monster
{

private:
	friend MapEditor;

	AniRender* m_CoralKeytar;
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
	Keytar(CVector _TargetPos);
	~Keytar();


private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;

	void Move();
	void ChangePos(CVector _Pos);

public:

	void Update() override;

};

