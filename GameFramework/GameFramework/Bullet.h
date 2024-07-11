#pragma once
#include "GameActor.h"

class Bullet : public GameActor
{
public:
	float Time;

public:
	void Update() override;
	void Dir(CVector _Dir);

private:
	CVector m_Dir;

public:
	Bullet();
	~Bullet();
};

