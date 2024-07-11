#pragma once
#include "Monster.h"


class MapEditor;
class Monkey :
	public Monster
{
private:
	AniRender* m_Monkey;
	CVector::MapIndex nextIndex;
	std::map<int, CVector::MapIndex> nextMap;
	bool m_ChangeHealth;
	bool m_OnPlayer;
	int m_HitCount;

	std::wstring MonkeyDeath;
	std::wstring MonkeyGrab;
	std::wstring MonkeyHit[2];


public:
	Monkey(CVector _TargetPos);
	~Monkey();

public:
	friend MapEditor;
	void Update() override;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;
	void PlaySound(MonsterSound _SoundType) override;
	void InitSound();
	void OnPlayer();

	void Move();
	void ChangePos(CVector _Pos);
	bool CheckIndex();

};

