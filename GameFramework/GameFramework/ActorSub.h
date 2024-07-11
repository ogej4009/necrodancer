#pragma once
#include "BaseUpdater.h"
#include <GameMath.h>
#include <map>
#include <list>

//Actor의 정보가 필요한 충돌, 랜더링 클래스에 Actor의 정보를 전달해주는 
//서브 클래스 이걸 활용해서 Player에게 알아서 붙어다니는 이미지를 만듦
class GameActor;
class ActorSub : public BaseUpdater
{
protected:

	//State의 Cam을 사용해서 위치정보를 바꿀것인지 여부
	bool m_IsCam;
	//부모 Actor에 대한 정보
	GameActor* m_pParent;
	//해당 자식들이 개별적으로 가지고 있을 크기와 위치
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

	//기준점이 되주는 곳으로부터의 위치
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

