//#include "stdafx.h"
#include "GameCol.h"
#include <GameMacro.h>
#include <Windows.h>
#include <GameTexMgr.h>
#include "GameActor.h"


GameCol::GameCol()
{
}

GameCol::~GameCol()
{
}


//static bool RECTTORECT(const HRECT& _Left, const HRECT& _Right)
//ColCheckFunc[0][0] -> &HGAMEMATH::RECTTORECT�� �ּҷ� ȣ��
//�Լ��������� ���� ��� void(TEST::*MemFunc2)() = &Test::Func ����

bool(__stdcall *GameCol::ColCheckFunc[static_cast<int>(COLTYPE::COLTYPE_MAX)][static_cast<int>(COLTYPE::COLTYPE_MAX)])(const GameRect& _Left, const GameRect& _Right)
{
	//{ &GameMath::RECTTORECT, &GameMath::RECTTOCIRCLE, &GameMath::RECTTOPOINT },
	//{ &GameMath::CIRCLETORECT, &GameMath::CIRCLETOCIRCLE, &GameMath::CIRCLETOPOINT },
	//{ &GameMath::POINTTORECT, &GameMath::POINTTOCIRCLE, &GameMath::POINTTOPOINT }
};

//Create Col���� �ʱ�ȭ ����
void GameCol::Init()
{
	switch (m_Type)
	{
	case COLTYPE::RECT2D:
		DebugRenderFunc = &GameCol::RectRender;
		break;
	case COLTYPE::CIRCLE2D:
		DebugRenderFunc = &GameCol::CirCleRender;
		break;
	case COLTYPE::POINT2D:
		DebugRenderFunc = &GameCol::PointRender;
		break;
	case COLTYPE::COLTYPE_MAX:
		ASSERTMSG;
		break;
	default:
		break;
	}
}


void GameCol::ColCheck(GameCol* _Other)
{
	//���� ColCheck�� �Լ������� ���·� ���ȴ�.
	if (true == ColCheckFunc[static_cast<int>(m_Type)][static_cast<int>(_Other->m_Type)](CalRect(), _Other->CalRect()))
	{
		if (m_ColSet.end() == m_ColSet.find(_Other))
		{
			//ù ������ ���� �����͸� �߰����༭_�ؽ��ڵ�� ����.
			//�浹 ���� �����Ϳ� �߰�
			//���� ���� ��� ����ְ� �ؼ� �װų� update Off�� ������
			//�� �ּҰ��� �ٰ� �ڽ��� ���̺��� ������ �Ѵ�.
			//ColEnter���� virtual�̴�.
			m_ColSet.insert(_Other);
			_Other->m_ColSet.insert(this);
			ParentActor()->ColEnter(this, _Other);
			_Other->ParentActor()->ColEnter(_Other, this);
		}
		else
		{
			//�̹� ���̺� �����ϴµ� �� �浹üũ�� ���Դ�.
			ParentActor()->ColStay(this, _Other);
			_Other->ParentActor()->ColStay(_Other, this);
		}
	}
	else
	{
		//���̺� �����ϴ� ģ���� �浹üũ�� �ȵ��Դ�.
		if (m_ColSet.end() != m_ColSet.find(_Other))
		{
			//�����ְ� ����
			m_ColSet.erase(_Other);
			_Other->m_ColSet.erase(this);
			ParentActor()->ColExit(this, _Other);
			_Other->ParentActor()->ColExit(_Other, this);
		}
	}
}


void GameCol::DebugRender()
{
	// �ɹ��Լ� ȣ���ϴ¹�
	// �տ� �� �Լ��� ȣ���Ҽ��ִ� ��ü�� �ʿ��ϴ�
	// �����ʹϱ�./
	(this->*DebugRenderFunc)();
}


void GameCol::RectRender()
{
	GameRect Fi = CalRect();
	Rectangle(GameTexMgr::BackDC(), Fi.IL(), Fi.IT(), Fi.IR(), Fi.IB());
}

void GameCol::CirCleRender()
{

}

void GameCol::PointRender()
{

}