#pragma once
#include "GameActor.h"


class StairLock :	public GameActor
{
public:
	friend MapEditor;

private:
	GameRender* m_Stair;

public:
	StairLock(CVector _TargetPos);
	~StairLock();

public:
	void Update() override;
};


