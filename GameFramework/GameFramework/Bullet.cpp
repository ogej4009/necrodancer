//#include "stdafx.h"
#include "Bullet.h"
#include "GameState.h"
#include <GameInput.h>
#include <GameTime.h>
#include <math.h>

#define PI 3.14159265

Bullet::Bullet() : Time(0.0f), m_Dir(CVector::DELTARIGHT())
{
	m_ActorTypeIndex = EActorTypeIndex::InteractObject;
	//Size({ 50, 50 });
	Pos({ 100, 100 });
	GameRender* NewSetting = CreateRender<GameRender>(-10);
	NewSetting->SubSize({ 30,30 });
}


Bullet::~Bullet()
{
}

void Bullet::Dir(CVector _Dir)
{
	m_Dir = CVector::UnitVectorAB(_Dir, Pos());
}


void Bullet::Update()
{
	Time += GameTime::DeltaTime();
	if (Time >= 5)
	{
		Death();
	}

	Move(m_Dir * 100 * GameTime::DeltaTime());
}