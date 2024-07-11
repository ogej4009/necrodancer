#pragma once
#include "GameActor.h"

class MapEditor;
class GameWall : public GameActor
{
public:
	friend MapEditor;

private:
	int m_Def;
	GameRender* m_Wall;
	CVector m_CurPos;
	CVector m_RealPos;
	int m_WallIndex;
	int m_CutCount;
	int m_Health;
	wchar_t m_DebugText[32];


public:
	GameWall(CVector _TargetPos, std::wstring _WallName);
	~GameWall();

public:
	void ChangeIndex(int _Index);
	void Update() override;
	void DebugRender() override;
	int Def()
	{
		return m_Def;
	}
};

