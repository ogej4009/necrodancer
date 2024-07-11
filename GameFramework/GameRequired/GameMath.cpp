#include "GameMath.h"
#include "GameTime.h"
#include "GameMacro.h"



//단위벡터
CVector CVector::DLEFT;
CVector CVector::DRIGHT;
CVector CVector::DUP;
CVector CVector::DDOWN;
CVector CVector::DZERO;

CVector  const CVector::LEFT = { -1.0f, 0.0f };
CVector  const CVector::RIGHT = { 1.0f, 0.0f };
CVector  const CVector::UP = { 0.0f, -1.0f };
CVector  const CVector::DOWN = { 0.0f, 1.0f };
CVector  const CVector::ZERO = { 0.0f, 0.0f };

//DELTADIR을 배열로 선언해서 0~4까지 방향을 넣어놓음
CVector CVector::st_DELTADIR[static_cast<int>(eDir::DIRMAX)];
GameRandom CVector::RANDOM;
//GAMEWiN의 base루프에서 데드타임에 같이 돌아간다.
void CVector::DIRUPDATE()
{
	DLEFT = LEFT * GameTime::DeltaTime();
	DRIGHT = RIGHT * GameTime::DeltaTime();
	DUP = UP * GameTime::DeltaTime();
	DDOWN = DOWN * GameTime::DeltaTime();
	DZERO = CVector::ZERO;

	st_DELTADIR[static_cast<int>(eDir::DIRL)] = DLEFT;
	st_DELTADIR[static_cast<int>(eDir::DIRR)] = DRIGHT;
	st_DELTADIR[static_cast<int>(eDir::DIRT)] = DUP;
	st_DELTADIR[static_cast<int>(eDir::DIRB)] = DDOWN;
	st_DELTADIR[static_cast<int>(eDir::DIRNONE)] = DZERO;

}


CVector CVector::Lerp(CVector _Start, CVector _End, float _DeltaTime)
{
	if (_DeltaTime > 1)
	{
		_DeltaTime = 1.0f;
	}
	return  _Start * (1 - _DeltaTime) + _End * _DeltaTime;
}


float CVector::GetVectorLength(CVector _Vector)
{
	float vectorLength = (_Vector.X * _Vector.X) + (_Vector.Y * _Vector.Y);
	vectorLength = static_cast<float>(sqrt(vectorLength));
	return  vectorLength;
}

CVector CVector::UnitVectorAB(CVector _VectorB, CVector _VectorA)
{
	CVector vectorAB = _VectorB - _VectorA;
	float vectorABLength = GetVectorLength(vectorAB);

	if (vectorABLength == 0)
	{
		ASSERTMSG;
		return CVector(0, 0);
	}

	CVector unitVectorAB = vectorAB * (1.0f / vectorABLength);
	return unitVectorAB;
}

CVector CVector::UnitVectorA(CVector _Vector)
{
	float vectorALength = GetVectorLength(_Vector);

	if (vectorALength == 0)
	{
		ASSERTMSG;
		return CVector(0, 0);
	}

	CVector unitVectorA = _Vector * (1.0f / vectorALength);
	return unitVectorA;
}


GameMath::GameMath()
{
}


GameMath::~GameMath()
{
}
