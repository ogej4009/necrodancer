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
//ColCheckFunc[0][0] -> &HGAMEMATH::RECTTORECT를 주소로 호출
//함수포인터의 원래 용법 void(TEST::*MemFunc2)() = &Test::Func 형태

bool(__stdcall *GameCol::ColCheckFunc[static_cast<int>(COLTYPE::COLTYPE_MAX)][static_cast<int>(COLTYPE::COLTYPE_MAX)])(const GameRect& _Left, const GameRect& _Right)
{
	//{ &GameMath::RECTTORECT, &GameMath::RECTTOCIRCLE, &GameMath::RECTTOPOINT },
	//{ &GameMath::CIRCLETORECT, &GameMath::CIRCLETOCIRCLE, &GameMath::CIRCLETOPOINT },
	//{ &GameMath::POINTTORECT, &GameMath::POINTTOCIRCLE, &GameMath::POINTTOPOINT }
};

//Create Col에서 초기화 해줌
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
	//밑의 ColCheck는 함수포인터 형태로 사용된다.
	if (true == ColCheckFunc[static_cast<int>(m_Type)][static_cast<int>(_Other->m_Type)](CalRect(), _Other->CalRect()))
	{
		if (m_ColSet.end() == m_ColSet.find(_Other))
		{
			//첫 조우라면 상대방 포인터를 추가해줘서_해쉬코드로 쓴다.
			//충돌 관리 포인터에 추가
			//나와 상대방 모두 들고있게 해서 죽거나 update Off가 됬을때
			//그 주소값에 다가 자신을 테이블에서 지우라고 한다.
			//ColEnter등은 virtual이다.
			m_ColSet.insert(_Other);
			_Other->m_ColSet.insert(this);
			ParentActor()->ColEnter(this, _Other);
			_Other->ParentActor()->ColEnter(_Other, this);
		}
		else
		{
			//이미 테이블에 존재하는데 또 충돌체크를 들어왔다.
			ParentActor()->ColStay(this, _Other);
			_Other->ParentActor()->ColStay(_Other, this);
		}
	}
	else
	{
		//테이블에 존재하는 친구가 충돌체크를 안들어왔다.
		if (m_ColSet.end() != m_ColSet.find(_Other))
		{
			//지워주고 빠이
			m_ColSet.erase(_Other);
			_Other->m_ColSet.erase(this);
			ParentActor()->ColExit(this, _Other);
			_Other->ParentActor()->ColExit(_Other, this);
		}
	}
}


void GameCol::DebugRender()
{
	// 맴버함수 호출하는법
	// 앞에 그 함수를 호출할수있는 객체가 필요하다
	// 포인터니까./
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