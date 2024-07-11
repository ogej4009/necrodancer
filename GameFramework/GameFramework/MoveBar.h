#pragma once
#include "GameActor.h"


class MoveBar : public GameActor
{
private:
	GameRender* m_MoveBar[4];
	CVector m_Position;
	CVector m_HeartPosition;
	CVector CurPos[4];
	CVector NextPos[4];
	float m_Timer;
	bool m_bIsRight;
	int m_MarkerPos[4];
	float m_MoveRange;

public:
	MoveBar(bool _IsLeft);
	~MoveBar();

private:
	void InitRender(CVector _Pos, int _Index);
	void barMove();


public:
	virtual void Update() override;
};

