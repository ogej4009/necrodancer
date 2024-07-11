#pragma once
#include "GameActor.h"


class MapEditor;
class Stair : public GameActor
{
public:
	friend MapEditor;

private:
	GameRender* m_Stair;

public:
	Stair(CVector _TargetPos);
	~Stair();

public:
	void Update() override;
};

