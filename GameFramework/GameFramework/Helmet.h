#pragma once
#include "GameActor.h"


class Helmet :	public GameActor
{
private:
	GameRender* m_Helmet;
	int m_Len;
	friend MapEditor;
	bool m_OnBuying;

public:
	Helmet(CVector _Pos);
	~Helmet();


public:
	void Update() override;
	void ShopItem(bool _IsBuying = true)
	{
		m_OnBuying = _IsBuying;
	}
};

