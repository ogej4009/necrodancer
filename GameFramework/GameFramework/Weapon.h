#pragma once
#include "GameActor.h"


class MapEditor;
class Weapon : public GameActor
{
public:
	enum class WeaponType
	{
		Dagger,
		Rapier,
		LongSword,
		BroadSword
	};

	friend MapEditor;
private:
	WeaponType m_WeaponType;
	GameRender* m_Weapon;
	bool m_bCanChange;
	int m_Len;
	bool m_OnBuying;

public:
	Weapon(CVector _Pos, WeaponType _WeaponType);
	~Weapon();

public:
	void Update() override;
	int CurType(int _WeaponType);
	void ShopItem(bool _IsBuying = true)
	{
		m_OnBuying = _IsBuying;
	}

private:
	void InitRender(CVector _Pos);
	void SwtichImg();
	
};

