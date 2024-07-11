#pragma once
#include "GameActor.h"


class Chest : public GameActor
{

private:
	GameRender* m_Chest;
	int m_WeaponType;
	int m_Len;
public:
	Chest(CVector _TargetPos, int _WeaponType);
	~Chest();


public:
	friend MapEditor;
	void Update() override;
	void OpenChest();
	int InWeapon()
	{
		return m_WeaponType;
	}

};

