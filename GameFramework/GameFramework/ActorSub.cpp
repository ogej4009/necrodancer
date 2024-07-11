//#include "stdafx.h"
#include "ActorSub.h"
#include "GameActor.h"
#include "GameState.h"

ActorSub::ActorSub() :m_SubPos(CVector::ZERO), m_SubSize(CVector::ZERO), m_IsCam(true)
{
}


ActorSub::~ActorSub()
{
}

bool ActorSub::IsUpdate()
{
	return m_pParent->IsUpdate() && BaseUpdater::IsUpdate();
}


bool ActorSub::IsDeath()
{
	return m_pParent->IsDeath() || BaseUpdater::IsDeath();
}

//자신의 부모가 있다면 부모의 위치에서 벡터로 더한 위치를 가지게 됨
//자신이 부모라면 그냥 자신의 위치 POS()만 가짐
//부모에 딸린 개체라면 SubPos를 붙여주면 알아서 더해준 위치로 이동한다.
CVector ActorSub::CalPos()
{
	if (true == m_IsCam)
	{
		return m_pParent->Pos() + m_SubPos - (m_pParent->State()->CamPos());
	}

	return m_pParent->Pos() + m_SubPos;
}

//최종 위치와 해당하는 크기를 반환해줌
GameRect ActorSub::CalRect()
{
	//pos랑 size 둘다 pubic 이라서 가능한 선언
	return { CalPos(), m_SubSize };
}
