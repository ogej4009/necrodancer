#pragma once
#include "GameActor.h"


class MapEditor;
class GameBackWall : public GameActor
{
public:
	friend MapEditor;

private:
	GameRender* m_BackWall;
	int m_WallIndex;
	int m_BackWallIndex;
	int m_CutCount;


public:
	GameBackWall(CVector _TargetPos, std::wstring _WallName);
	~GameBackWall();

public:
	void ChangeIndex(int _Index);
	void Update() override;
};

