#pragma once
#include "Monster.h"

class MapEditor;
class BlueSlime :
	public Monster
{
private:
	AniRender* m_BlueSlime;
	CVector::MapIndex m_FindPlayer;
	std::map<__int64, GameActor*>::iterator m_NextIter;
	std::map<int, CVector::MapIndex> nextMap;

	bool m_bIsEnd;
	

public:
	BlueSlime(CVector _TargetPos);
	~BlueSlime();

public:
	friend MapEditor;
	void Update() override;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;


	void InitSound();
	void PlaySound(MonsterSound _SoundType) override;
	void ChangePos(CVector _Pos);
	void Move();

	std::wstring BlueSlimeHit[3];
	std::wstring BlueSlimeDeath;
	std::wstring BlueSlimeAttack;
};

