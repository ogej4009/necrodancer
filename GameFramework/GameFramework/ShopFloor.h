#pragma once
#include "GameActor.h"


class ShopFloor :
	public GameActor
{
private:
	GameRender* m_ShopFloor;
	int m_Len;

public:
	ShopFloor(CVector _TargetPos);
	~ShopFloor();

public:
	friend MapEditor;


public:
	void Update() override;
};

