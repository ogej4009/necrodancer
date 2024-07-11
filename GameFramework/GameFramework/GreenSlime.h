#pragma once
#include "Monster.h"

class MapEditor;
class GreenSlime : public Monster
{
private:
	AniRender* m_GreenSlime;
	std::wstring BlueSlimeHit[3];
	std::wstring BlueSlimeDeath;

public:
	GreenSlime(CVector _TargetPos);
	~GreenSlime();

public:
	friend MapEditor;
	void Update() override;

private:
	void InitAni(CVector _TargetPos) override;
	void InitFSM() override;
	void ChangeState(MonsterState _State) override;
	void InitSound();
	void PlaySound(MonsterSound _SoundType) override;
};

