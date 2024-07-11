#pragma once
#include "GameActor.h"


class Armor :
	public GameActor
{
private:
	GameRender* m_Armor;
	int m_Len;
	friend MapEditor;
	bool m_OnBuying;

public:
	Armor(CVector _Pos);
	~Armor();


public:
	void Update() override; 
	void ShopItem(bool _IsBuying = true)
	{
		m_OnBuying = _IsBuying;
	}
};

