//#include "stdafx.h"
#include <string.h>
#include "GameActor.h"
#include "GameState.h"
#include "AniRender.h"
#include <GameWin.h>
#include <GameTex.h>
#include <GameTexMgr.h>
#include "MapEditor.h"


#ifdef RELEASEMODE
#ifdef X32
#pragma comment(lib, "GameRequired.lib")
#else X64
#pragma comment(lib, "GameRequired.lib")
#endif
#else DEBUGMODE
#ifdef X32
#pragma comment(lib, "GameRequired.lib")
#else X64
#pragma comment(lib, "GameRequired.lib")
#endif
#endif

#pragma comment(lib, "msimg32.lib")

std::map<__int64, GameActor*>::iterator GameActor::FindIter;
std::map<__int64, GameActor*>::iterator GameActor::EndIter;



const float GameActor::m_TileSize = 40.0f;
const float GameActor::m_TileSizeHalf = 20.0f;
const CVector GameActor::m_TileSize_Vector = {40.0f, 40.0f};

GameActor::GameActor() : m_bDeath(false), m_TilePadding(40.0f), m_MapIndex({ 0,0 })
{
	//State의 m_Allvector에 포함시켜서 관리한다.
	GameState::InActor(this);
}


GameActor::~GameActor()
{
	RenderStartIter = m_AllRenderList.begin();
	RenderEndIter = m_AllRenderList.end();

	for (; RenderStartIter != RenderEndIter; ++RenderStartIter)
	{
		if (*RenderStartIter != nullptr)
		{
			delete *RenderStartIter;
			*RenderStartIter = nullptr;
		}
	}

	ColStartIter = m_AllColList.begin();
	ColEndIter = m_AllColList.end();

	for (; ColStartIter != ColEndIter; ++ColStartIter)
	{
		if (*ColStartIter != nullptr)
		{
			delete *ColStartIter;
			*ColStartIter = nullptr;
		}
	}
	m_AllColList.clear();
}


void GameActor::Update()
{

}

void GameActor::DebugRender()
{

}

void GameActor::AddRenderer(GameRender* _Render)
{
	m_State->AddRenderer(_Render);
	_Render->InitFindPlayer();
}

CVector GameActor::ActorCalcPos(CVector _InitPos)
{
	return _InitPos - State()->CamPos();
}


//State의 Release()가 같이 돌려준다.
void GameActor::Release()
{
	RenderStartIter = m_AllRenderList.begin();
	RenderEndIter = m_AllRenderList.end();

	for (; RenderStartIter != RenderEndIter;)
	{
		if (true == (*RenderStartIter)->IsDeath())
		{
			delete *RenderStartIter;
			RenderStartIter = m_AllRenderList.erase(RenderStartIter);
			continue;
		}
		++RenderStartIter;
	}
}

void GameActor::Death()
{
	m_bDeath = true;


}

//
//GameCol* GameActor::CreateCol(GameCol::COLTYPE _Type, int _League)
//{
//
//	GameCol* NewCol = new GameCol();
//	NewCol->m_pParent = this;
//	NewCol->m_Type = _Type;
//	NewCol->m_League = _League;
//
//	NewCol->Init();
//	m_State->AddCol(NewCol);
//	m_AllColList.push_back(NewCol);
//
//	return NewCol;
//
//}

CVector GameActor::CalPos()
{
	return Pos() - m_State->CamPos();
}