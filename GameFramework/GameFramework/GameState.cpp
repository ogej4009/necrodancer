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

//m_Instant�� �Ҹ�� �����ϸ� ��ü State�� ������ ���
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

//Actor�� ������ ���������
void GameState::InActor(GameActor* _Actor)
{
	// ������Ʈ�� ��������� ���� ����.
	if (nullptr != m_LoadingState)
	{
		_Actor->m_State = m_LoadingState;
		m_LoadingState->m_AllActor.push_back(_Actor);
	}
	// �÷��̵��� �ǽð� ����.
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
	//�ð� ����
	m_CamTimer += GameTime::DeltaTime();

	//������� Ű�� ����
	if (GameInput::Down(L"DebugTurn"))
	{
		GameState::ChangeDebug();
	}

	// ��������
	m_CurState->StateUpdate();
	//m_AllActor�� �ִ� �� ��ȸ�ϸ鼭 update, render�� ������
	m_CurState->ActorUpdate();
	m_CurState->MoveInterface();
	//����� �ʱ�ȭ(�����δ� ��� �̹���)
	Rectangle(GameTexMgr::BackDC(), 0, 0, GameTexMgr::BackDCSize().IX(), GameTexMgr::BackDCSize().IY());

	m_CurState->ActorRender();

	//�浹�� ����� ����
	if (true == m_bIsDebug)
	{
		//m_CurState->ColDebugRender();
		m_CurState->ActorDebugRender();
		m_CurState->MoveDebugRender();
	}

	//����ۿ��� �׸��� ���⿡�� �ѹ� �׷��ְ� ��(��¦�� ���� ����)
	BitBlt(GameWin::MainHDC(), 0, 0, GameTexMgr::BackDCSize().IX(), GameTexMgr::BackDCSize().IY(), GameTexMgr::BackDC(), 0, 0, SRCCOPY);
	//�浹üũ
	//m_CurState->Collision();

	// ����� ���� ������Ʈ �ϴ� ���� ������ �ʴ´�.
	m_CurState->Release();
}



//////////////////////////////////// Member

GameState::GameState() : m_GoldIndex(0)
{

}


GameState::~GameState() //�Ҹ�� m_Allactor �޸� ����
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
		//�θ� �ڽſ� ���� �������� ���������� ������ �ְ� �ȴ�.
		//�� ���� Actor�� �Ҹ��ڰ� ���鼭 �����ش�.
		//State������ �ش� ����Ʈ�� Ŭ�������ִ� ������ ��
		m_AllRenderLeague.clear();
	}

}



//progress�� �����ִ� �Լ�
void GameState::StateUpdate() {



}

//CreateState���� ���λ����� State(Scene)�� Loading�� ȣ������
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

		// ȭ�� �ٱ��� �ִ� �ֵ��� �浹 �Ƚ�Ű�� ���
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
				// �ٸ� �׷�� �浹�ؾ� �Ҷ�.
				CheckLeagueCol(Src, Other);
			}
			else
			{
				CheckLeagueCol(Src);
				// �ڽ��� �ڽŰ� �浹�Ҷ�. �浹�Ҷ�.
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


//progress�� �����ִ� �Լ�
void GameState::ActorRender()
{
	std::map<int, std::list<GameRender*>>::iterator StartLeague = m_AllRenderLeague.begin();
	std::map<int, std::list<GameRender*>>::iterator EndLeague = m_AllRenderLeague.end();
	//AllRenderLeague�� �ѹ��� ��ȸ�Ѵ�.
	//Order �������
	for (; StartLeague != EndLeague; ++StartLeague)
	{
		//AllRenderLeague�� List�� �ѹ��� ��ȸ�Ѵ�.
		//�Է¼������
		std::list<GameRender*>::iterator RenderStart = StartLeague->second.begin();
		std::list<GameRender*>::iterator RenderEnd = StartLeague->second.end();

		for (; RenderStart != RenderEnd; ++RenderStart)
		{

			if (false == (*RenderStart)->IsUpdate())
			{
				//�ش� �������� ���¸� Ȯ���ؼ� �ƴѰ�� ���� �Ѿ��
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


//�浹�ؾ��� �׷��� �̾��ִ� ��
//Src�׷��� Dest�׷�� �浹�� üũ�ϰ� �ȴ�.
//State�� ��ӹ��� �׷���� Loading()�Ҷ� �ʱ�ȭ ����� �Ѵ�.
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

//progress�� �����ִ� �Լ�
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
	//�÷��̾� �̵��� �����ִ� �Լ�
}

//progress�� �����ִ� �Լ�
void GameState::Release() {

	// �θ�� �׾��µ�
	// �������� �Ⱥ�����.

	// �θ���� �ױ����� �ڽź��� ó���Ѵ�.
	// ������ ������ �� üũ�ϱ�
	std::map<int, std::list<GameRender*>>::iterator StartLeague = m_AllRenderLeague.begin();
	std::map<int, std::list<GameRender*>>::iterator EndLeague = m_AllRenderLeague.end();

	for (; StartLeague != EndLeague; ++StartLeague)
	{
		std::list<GameRender*>::iterator RenderStart = StartLeague->second.begin();
		std::list<GameRender*>::iterator RenderEnd = StartLeague->second.end();

		for (; RenderStart != RenderEnd;)
		{
			// �θ� �׾��ų�
			// �θ����(����)�� ������ �ڻ��ϰ� ��ɳ�������.
			if (true == (*RenderStart)->IsDeath())
			{
				RenderStart = StartLeague->second.erase(RenderStart);
				continue;
			}
			++RenderStart;
		}
	}

	//CollLeague�� Release�� üũ�ؼ� �����ֱ�
	//m_AllColLeague ��� �ݸ����� ������ Ÿ���� Ű�� ������ ����
	DebugStart = m_AllColLeague.begin();
	DebugEnd = m_AllColLeague.end();

	for (; DebugStart != DebugEnd; ++DebugStart)
	{
		DebugListStart = DebugStart->second.begin();
		DebugListEnd = DebugStart->second.end();

		//����ݸ����� �������
		for (; DebugListStart != DebugListEnd; )
		{
			if ((nullptr != *DebugListStart) &&
				true == (*DebugListStart)->IsDeath())
			{
				// �ش� �ݸ����� ����� �� ���������� �Ѿ��(earse�� �������� ����)
				// �ش� �׸��� ����°� �ƴ϶� list�� ���ΰ��� ����ٴ� �Ϳ� ����!
				DebugListStart = DebugStart->second.erase(DebugListStart);
				continue;
			}
			++DebugListStart;
		}
	}


	//Actor�� Release�� üũ�ؼ� �����ֱ�
	ProgressStart = m_AllActor.begin();
	ProgressEnd = m_AllActor.end();
	// ����Ʈ�� ���ﶧ�� 
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
	//���׿� �ش� ������ �켱������ ��ϵ� ������ ����Ʈ�� �����ϴ��� Ȯ��
	std::map<int, std::list<GameRender*>>::iterator FindIter = m_AllRenderLeague.find(_Render->Order());

	//�������� �ʴ´ٸ� ����Ʈ�� �ϳ� �߰����� �׸��� �� ���� ����Ʈ�� �߰�
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
		//�Ⱥ���ٸ� ������.
		if (m_Gold[m_GoldIndex] != nullptr)
		{
			ASSERTMSG;
		}
	}

	m_Gold[m_GoldIndex] = new Gold(_MapIndex, GoldCount);
	++m_GoldIndex;

}

