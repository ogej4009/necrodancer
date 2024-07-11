#pragma once
#include "BaseUpdater.h"
#include <GameMath.h>
#include <map>
#include <list>

//Actor�� ������ �ʿ��� �浹, ������ Ŭ������ Actor�� ������ �������ִ� 
//���� Ŭ���� �̰� Ȱ���ؼ� Player���� �˾Ƽ� �پ�ٴϴ� �̹����� ����
class GameActor;
class ActorSub : public BaseUpdater
{
protected:

	//State�� Cam�� ����ؼ� ��ġ������ �ٲܰ����� ����
	bool m_IsCam;
	//�θ� Actor�� ���� ����
	GameActor* m_pParent;
	//�ش� �ڽĵ��� ���������� ������ ���� ũ��� ��ġ
	CVector m_SubPos;
	CVector m_SubSize;

	CVector::MapIndex m_SubIndex;

public:
	GameActor* ParentActor()
	{
		return m_pParent;
	}

public:
	CVector CalPos();
	GameRect CalRect();
	CVector::MapIndex SubPosIndeX()
	{
		return m_SubIndex;
	}

	void TurnOffCam()
	{
		m_IsCam = false;
	}

	//�������� ���ִ� �����κ����� ��ġ
	virtual void SubPos(CVector _SubPos) 
	{
		m_SubPos = _SubPos;
	}

	void SubSize(CVector _SubSize) 
	{
		m_SubSize = _SubSize;
	}

	virtual CVector SubPos()
	{
		return m_SubPos;
	}

	CVector SubSize()
	{
		return m_SubSize;
	}

	virtual void SubPosPure(CVector _Pos)
	{
		m_SubPos = _Pos;
	};

public:
	virtual bool IsUpdate() override;
	virtual bool IsDeath() override;

public:
	ActorSub();
	virtual ~ActorSub();
};

