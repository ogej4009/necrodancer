#pragma once
#include "GameActor.h"


class Gold :
	public GameActor
{
private:
	GameRender* m_Gold;
	int m_GoldCount;
	int m_Len;

public:
	Gold(CVector::MapIndex _MapIndex, int _Gold);
	~Gold();
	
private:
	void ChangeImg(int _Gold);

public:
	void Update() override;
};

