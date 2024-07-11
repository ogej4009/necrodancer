#pragma once
#include "GameActor.h"


class CenterHeart : public GameActor
{
private:
	std::list<GameRender*> m_Hud;
	CVector m_Position;

	enum {
		IDLE,
	};
public:
	CenterHeart();
	~CenterHeart();

public:
	virtual void Update() override;
	void InitRender(CVector _Pos);

};

