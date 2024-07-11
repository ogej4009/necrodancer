//#include "stdafx.h"
#include "GameState.h"
#include "GameActor.h"
#include <GameMacro.h>
#include <GameWin.h>
#include <GameTexMgr.h>
#include <GameInput.h>
#include "GameTime.h"
#include "GameMap.h"
#include <GameMath.h>
#include "Player.h"
#include "Monster.h"
#include "Heart.h"
#include "Diamond_UI.h"
#include "Gold_UI.h"
#include "HUD.h"
#include "CenterHeart.h"
#include "MoveBar.h"
#include "Gold.h"

std::map<std::wstring, GameState*> GameState::m_AllState;
GameState* GameState::m_LoadingState = nullptr;
GameState* GameState::m_CurState = nullptr;

GameState::STATICCONDES GameState::m_Inst;
CVector GameState::m_DefaultCam = CVector({ -480,-270 });
CVector GameState::m_Cam = CVector({ -480,-270 });
CVector GameState::m_CurCamPos = CVector({ -480,-270 });
CVector GameState::m_NextCamPos = CVector({ -480,-270 });
float GameState::m_CamTimer = 0.0f;
bool GameState::m_bIsDebug = true;
//960,540 -> 480,270

//m_Instant의 소멸로 동작하며 전체 State의 삭제를 담당
GameState::STATICCONDES::~STATICCONDES()
{
	std::map<std::wstring, GameState*>::iterator StartIter = GameState::m_AllState.begin();
	std::map<std::wstring, GameState*>::iterator EndIter = GameState::m_AllState.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	GameState::m_AllState.clear();
}

//Actor의 동작을 진행시켜줌
void GameState::InActor(GameActor* _Actor)
{
	// 스테이트가 만들어지는 순간 생성.
	if (nullptr != m_LoadingState)
	{
		_Actor->m_State = m_LoadingState;
		m_LoadingState->m_AllActor.push_back(_Actor);
	}
	// 플레이도중 실시간 생성.
	else if (nullptr != m_CurState)
	{
		_Actor->m_State = m_CurState;
		m_CurState->m_AllActor.push_back(_Actor);
	}

}


GameState* GameState::FindState(const wchar_t* _Name)
{
	std::map<std::wstring, GameState*>::iterator FindIter = m_AllState.find(_Name);

	if (FindIter == m_AllState.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GameState::ChangeState(const wchar_t* _Name)
{
	m_CurState = FindState(_Name);
	m_CurState->SoundOn();
	m_CurState->CamPos(m_DefaultCam);

	if (m_CurState->GetPlayer() != nullptr)
	{
		m_CurState->GetPlayer()->CheckItem();
	}
	
	m_CurState->m_NextCamPos = m_DefaultCam;
	m_CurState->m_CurCamPos = m_DefaultCam;
	m_CurState->m_StateChanged = true;
	
	if (nullptr == m_CurState)
	{
		ASSERTMSG;
	}

}

void GameState::SoundOn()
{

}

void GameState::SoundPause()
{

}

void GameState::Progress()
{
	//시간 진행
	m_CamTimer += GameTime::DeltaTime();

	//디버깅모드 키고 끄기
	if (GameInput::Down(L"DebugTurn"))
	{
		GameState::ChangeDebug();
	}

	// 한프레임
	m_CurState->StateUpdate();
	//m_AllActor에 있는 걸 순회하면서 update, render를 눌러줌
	m_CurState->ActorUpdate();
	m_CurState->MoveInterface();
	//백버퍼 초기화(실제로는 배경 이미지)
	Rectangle(GameTexMgr::BackDC(), 0, 0, GameTexMgr::BackDCSize().IX(), GameTexMgr::BackDCSize().IY());

	m_CurState->ActorRender();

	//충돌용 디버그 랜더
	if (true == m_bIsDebug)
	{
		//m_CurState->ColDebugRender();
		m_CurState->ActorDebugRender();
		m_CurState->MoveDebugRender();
	}

	//백버퍼에서 그린걸 여기에서 한번 그려주고 끝(반짝임 현상 제거)
	BitBlt(GameWin::MainHDC(), 0, 0, GameTexMgr::BackDCSize().IX(), GameTexMgr::BackDCSize().IY(), GameTexMgr::BackDC(), 0, 0, SRCCOPY);
	//충돌체크
	//m_CurState->Collision();

	// 절대로 저는 업데이트 하는 도중 지우지 않는다.
	m_CurState->Release();
}



//////////////////////////////////// Member

GameState::GameState() : m_GoldIndex(0)
{

}


GameState::~GameState() //소멸시 m_Allactor 메모리 해제
{
	Release();
	std::list<GameActor*>::iterator StartIter = m_AllActor.begin();
	std::list<GameActor*>::iterator EndIter = m_AllActor.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != *StartIter)
		{
			delete *StartIter;
			*StartIter = nullptr;
		}
	}

	m_AllActor.clear();


	if (m_AllRenderLeague.size() != 0)
	{
		//부모 자신에 대한 렌더러를 마지막까지 가지고 있게 된다.
		//이 값은 Actor의 소멸자가 돌면서 지워준다.
		//State에서는 해당 리스트를 클리어해주는 것으로 끝
		m_AllRenderLeague.clear();
	}

}



//progress가 돌려주는 함수
void GameState::StateUpdate() {



}

//CreateState에서 새로생성된 State(Scene)의 Loading을 호출해줌
void GameState::Loading() {



}

void GameState::CheckLeagueCol(
	std::map<int, std::list<GameCol*>>::iterator& _Src
	, std::map<int, std::list<GameCol*>>::iterator& _Other)
{
	SrcStart = _Src->second.begin();
	SrcEnd = _Src->second.end();

	for (; SrcStart != SrcEnd; ++SrcStart)
	{
		OtherStart = _Other->second.begin();
		OtherEnd = _Other->second.end();

		// 화면 바깥에 있는 애들은 충돌 안시키는 방법
		for (; OtherStart != OtherEnd; ++OtherStart)
		{
			(*SrcStart)->ColCheck(*OtherStart);
		}
	}
}



void GameState::InitUI()
{
	Heart* NewHeart = new Heart(10);
	Diamond_UI* NewDiamond = new Diamond_UI();
	Gold_UI* NewGold = new Gold_UI();
	HUD* NewHud = new HUD();
	CenterHeart* NewCentHeart = new CenterHeart();
	MoveBar* BarLeft = new MoveBar(false);
	MoveBar* BarRight = new MoveBar(true);

	m_UIList.push_back(static_cast<GameActor*>(NewHeart));
	m_UIList.push_back(static_cast<GameActor*>(NewDiamond));
	m_UIList.push_back(static_cast<GameActor*>(NewGold));
	m_UIList.push_back(static_cast<GameActor*>(NewHud));
	m_UIList.push_back(static_cast<GameActor*>(NewCentHeart));
	m_UIList.push_back(static_cast<GameActor*>(BarLeft));
	m_UIList.push_back(static_cast<GameActor*>(BarRight));

}

void GameState::TurnOffUI()
{
	for (auto &UIactor : m_UIList)
	{
		UIactor->Death();
	}
}

void GameState::Collision()
{
	// PLAYER
	// MONSTER
	// MONSTERATT

	// std::map<int   , std::set<int>        >
	//          PLAYER, (MONSTER, MONSTERATT)

	LinkStart = m_LinkData.begin();
	LinkEnd = m_LinkData.end();

	for (; LinkStart != LinkEnd; ++LinkStart)
	{
		OtherLinkStart = LinkStart->second.begin();
		OtherLinkEnd = LinkStart->second.end();

		//          LinkStart->first
		// std::map<int              , std::list<HGAMECOL*>>
		Src = m_AllColLeague.find(LinkStart->first);

		if (Src == m_AllColLeague.end())
		{
			continue;
		}

		// (MONSTER, MONSTERATT)
		for (; OtherLinkStart != OtherLinkEnd; ++OtherLinkStart)
		{
			Other = m_AllColLeague.find((*OtherLinkStart));

			if (Other == m_AllColLeague.end())
			{
				continue;
			}

			if (LinkStart->first != (*OtherLinkStart))
			{
				// 다른 그룹과 충돌해야 할때.
				CheckLeagueCol(Src, Other);
			}
			else
			{
				CheckLeagueCol(Src);
				// 자신이 자신과 충돌할때. 충돌할때.
			}
		}
	}

}
//
//void GameState::ColDebugRender()
//{
//	DebugStart = m_AllColLeague.begin();
//	DebugEnd = m_AllColLeague.end();
//
//	for (; DebugStart != DebugEnd; ++DebugStart)
//	{
//		DebugListStart = DebugStart->second.begin();
//		DebugListEnd = DebugStart->second.end();
//
//		for (; DebugListStart != DebugListEnd; ++DebugListStart)
//		{
//			(*DebugListStart)->DebugRender();
//		}
//	}
//}

//
//void GameState::AddCol(GameCol* _Render)
//{
//	std::map<int, std::list<GameCol*>>::iterator FindIter
//		= m_AllColLeague.find(_Render->m_League);
//
//	if (FindIter == m_AllColLeague.end())
//	{
//		m_AllColLeague.insert(
//			std::map<int, std::list<GameCol*>>::value_type(
//				_Render->m_League,
//				std::list<GameCol*>()
//			));
//
//		FindIter = m_AllColLeague.find(_Render->m_League);
//	}
//
//	FindIter->second.push_back(_Render);
//}


//progress가 돌려주는 함수
void GameState::ActorRender()
{
	std::map<int, std::list<GameRender*>>::iterator StartLeague = m_AllRenderLeague.begin();
	std::map<int, std::list<GameRender*>>::iterator EndLeague = m_AllRenderLeague.end();
	//AllRenderLeague를 한번씩 순회한다.
	//Order 순서대로
	for (; StartLeague != EndLeague; ++StartLeague)
	{
		//AllRenderLeague의 List를 한번씩 순회한다.
		//입력순서대로
		std::list<GameRender*>::iterator RenderStart = StartLeague->second.begin();
		std::list<GameRender*>::iterator RenderEnd = StartLeague->second.end();

		for (; RenderStart != RenderEnd; ++RenderStart)
		{

			if (false == (*RenderStart)->IsUpdate())
			{
				//해당 랜더러의 상태를 확인해서 아닌경우 랜더 넘어가기
				continue;
			}
			(*RenderStart)->Render();
		}
	}



	/*

		ProgressStart = m_AllActor.begin();
		ProgressEnd = m_AllActor.end();

		for (; ProgressStart != ProgressEnd; ++ProgressStart)
		{
			if (nullptr != *ProgressStart)
			{
				(*ProgressStart)->Render();
			}
		}*/
}

void GameState::ActorDebugRender()
{
	ProgressStart = m_AllActor.begin();
	ProgressEnd = m_AllActor.end();

	for (; ProgressStart != ProgressEnd; ++ProgressStart)
	{

		if (nullptr != *ProgressStart)
		{
			if (false == (*ProgressStart)->IsUpdate())
			{
				continue;
			}
			(*ProgressStart)->DebugRender();
		}
	}
}


//충돌해야할 그룹을 이어주는 것
//Src그룹은 Dest그룹과 충돌을 체크하게 된다.
//State를 상속받은 그룹들이 Loading()할때 초기화 해줘야 한다.
void GameState::Link(int _Src, int _Dest)
{
	std::map<int, std::set<int>>::iterator Find = m_LinkData.find(_Src);

	if (m_LinkData.end() == Find)
	{
		m_LinkData.insert(std::map<int, std::set<int>>::value_type(_Src, std::set<int>()));
		Find = m_LinkData.find(_Src);
	}

	std::set<int>::iterator FindDest = Find->second.find(_Dest);

	if (FindDest != Find->second.end())
	{
		ASSERTMSG;
	}

	Find->second.insert(_Dest);
}

//progress가 돌려주는 함수
void GameState::ActorUpdate()
{
	ProgressStart = m_AllActor.begin();
	ProgressEnd = m_AllActor.end();


	for (; ProgressStart != ProgressEnd; ++ProgressStart)
	{
		if (nullptr != *ProgressStart)
		{
			(*ProgressStart)->Update();
		}
	}
	//플레이어 이동을 도와주는 함수
}

//progress가 돌려주는 함수
void GameState::Release() {

	// 부모는 죽었는데
	// 랜더러는 안빠졌다.

	// 부모님이 죽기전에 자신부터 처리한다.
	// 랜더러 빠지는 거 체크하기
	std::map<int, std::list<GameRender*>>::iterator StartLeague = m_AllRenderLeague.begin();
	std::map<int, std::list<GameRender*>>::iterator EndLeague = m_AllRenderLeague.end();

	for (; StartLeague != EndLeague; ++StartLeague)
	{
		std::list<GameRender*>::iterator RenderStart = StartLeague->second.begin();
		std::list<GameRender*>::iterator RenderEnd = StartLeague->second.end();

		for (; RenderStart != RenderEnd;)
		{
			// 부모가 죽었거나
			// 부모님이(액터)가 나한테 자살하고 명령내렸을때.
			if (true == (*RenderStart)->IsDeath())
			{
				RenderStart = StartLeague->second.erase(RenderStart);
				continue;
			}
			++RenderStart;
		}
	}

	//CollLeague의 Release를 체크해서 돌려주기
	//m_AllColLeague 모든 콜리전과 각각의 타입을 키로 맵으로 저장
	DebugStart = m_AllColLeague.begin();
	DebugEnd = m_AllColLeague.end();

	for (; DebugStart != DebugEnd; ++DebugStart)
	{
		DebugListStart = DebugStart->second.begin();
		DebugListEnd = DebugStart->second.end();

		//모든콜리전을 대상으로
		for (; DebugListStart != DebugListEnd; )
		{
			if ((nullptr != *DebugListStart) &&
				true == (*DebugListStart)->IsDeath())
			{
				// 해당 콜리전을 지우고 그 다음것으로 넘어가기(earse는 다음인자 리턴)
				// 해당 항목을 지우는게 아니라 list의 내부값을 지운다는 것에 주의!
				DebugListStart = DebugStart->second.erase(DebugListStart);
				continue;
			}
			++DebugListStart;
		}
	}


	//Actor의 Release를 체크해서 돌려주기
	ProgressStart = m_AllActor.begin();
	ProgressEnd = m_AllActor.end();
	// 리스트를 지울때는 
	for (; ProgressStart != ProgressEnd; )
	{
		(*ProgressStart)->Release();

		if ((nullptr != *ProgressStart) &&
			(true == (*ProgressStart)->IsDeath()))
		{
			delete *ProgressStart;
			ProgressStart = m_AllActor.erase(ProgressStart);
			continue;
		}

		++ProgressStart;
	}
}

GameActor* GameState::FindPlayerOrNull()
{
	ProgressStart = m_AllActor.begin();
	ProgressEnd = m_AllActor.end();

	for (; ProgressStart != ProgressEnd; )
	{
		if ((*ProgressStart)->m_ActorTypeIndex == EActorTypeIndex::Player)
		{
			return *ProgressStart;
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}

void GameState::AddRenderer(GameRender* _Render)
{
	//리그에 해당 랜더의 우선순위로 등록된 랜더러 리스트가 존재하는지 확인
	std::map<int, std::list<GameRender*>>::iterator FindIter = m_AllRenderLeague.find(_Render->Order());

	//존재하지 않는다면 리스트를 하나 추가해줌 그리고 그 오더 리스트에 추가
	if (FindIter == m_AllRenderLeague.end())
	{
		m_AllRenderLeague.insert(
			std::map<int, std::list<GameRender*>>::value_type(
				_Render->Order(),
				std::list<GameRender*>())
		);

		FindIter = m_AllRenderLeague.find(_Render->Order());
	}

	FindIter->second.push_back(_Render);

}

void GameState::MoveDebugRender()
{
	/*wchar_t Arr[256] = { 0, };
	swprintf_s(Arr, L"Cam_Pos : x : %f, y : %f, Cur_CamPos X: %f, Y: %f,", m_Cam.X, m_Cam.Y, m_CurCamPos.X, m_CurCamPos.Y);
	TextOut(GameTexMgr::BackDC(), 0, 150, Arr, lstrlen(Arr));

	swprintf_s(Arr, L" Next_CamPos X:%f, Y:%f CamTimer %f", m_NextCamPos.X, m_NextCamPos.Y, m_CamTimer);
	TextOut(GameTexMgr::BackDC(), 0, 180, Arr, lstrlen(Arr));*/
}

void GameState::MoveInterface()
{
	m_Cam = CVector::Lerp(m_CurCamPos, m_NextCamPos, m_CamTimer * 3.0f);

	if (nullptr == m_Player)
	{
		return;
	}

	if (m_CamTimer < 0.34f)
	{
		return;
	}

	if (m_Player->CanMove())
	{
		if (m_Player->m_bPressLeft)
		{
			m_Player->m_bPressLeft = false;
			m_CamTimer = 0.0f;
			m_CurCamPos = m_Cam;
			m_NextCamPos = m_CurCamPos + CVector::LEFT * GameActor::TileSize();

			//m_Cam.X += CVector::LEFT.X * GameActor::TileSize();
		}

		if (m_Player->m_bPressRight)
		{
			m_Player->m_bPressRight = false;
			m_CamTimer = 0.0f;
			m_CurCamPos = m_Cam;
			m_NextCamPos = m_CurCamPos + CVector::RIGHT * GameActor::TileSize();
			//m_Cam.X += CVector::RIGHT.X * GameActor::TileSize();
		}

		if (m_Player->m_bPressUp)
		{
			m_Player->m_bPressUp = false;
			m_CamTimer = 0.0f;
			m_CurCamPos = m_Cam;
			m_NextCamPos = m_CurCamPos + CVector::UP * GameActor::TileSize();
			//m_Cam.Y += CVector::UP.Y * GameActor::TileSize();
		}

		if (m_Player->m_bPressDown)
		{
			m_Player->m_bPressDown = false;
			m_CamTimer = 0.0f;
			m_CurCamPos = m_Cam;
			m_NextCamPos = m_CurCamPos + CVector::DOWN * GameActor::TileSize();
			//m_Cam.Y += CVector::DOWN.Y * GameActor::TileSize();
		}
	}

}


void GameState::CreateGold(CVector::MapIndex _MapIndex, int GoldCount)
{
	if(m_GoldIndex > 19)
	{ 
		m_GoldIndex = 0;
		//안비웠다면 터진다.
		if (m_Gold[m_GoldIndex] != nullptr)
		{
			ASSERTMSG;
		}
	}

	m_Gold[m_GoldIndex] = new Gold(_MapIndex, GoldCount);
	++m_GoldIndex;

}

