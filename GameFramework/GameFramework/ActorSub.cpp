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

//�ڽ��� �θ� �ִٸ� �θ��� ��ġ���� ���ͷ� ���� ��ġ�� ������ ��
//�ڽ��� �θ��� �׳� �ڽ��� ��ġ POS()�� ����
//�θ� ���� ��ü��� SubPos�� �ٿ��ָ� �˾Ƽ� ������ ��ġ�� �̵��Ѵ�.
CVector ActorSub::CalPos()
{
	if (true == m_IsCam)
	{
		return m_pParent->Pos() + m_SubPos - (m_pParent->State()->CamPos());
	}

	return m_pParent->Pos() + m_SubPos;
}

//���� ��ġ�� �ش��ϴ� ũ�⸦ ��ȯ����
GameRect ActorSub::CalRect()
{
	//pos�� size �Ѵ� pubic �̶� ������ ����
	return { CalPos(), m_SubSize };
}
