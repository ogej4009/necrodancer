//#include "stdafx.h"
#include "Monster.h"
#include "ValueData.h"
#include <GameMath.h>
#include "GameCol.h"
#include <GameTexMgr.h>
#include "TileRender.h"
#include <GameSound.h>
#include <GameTime.h>




Monster::Monster() :
	m_Health(0)
	, m_Gold(0)
	, m_DefaultSize({ m_TileSize, m_TileSize })
	, m_Len(0)
	, m_bHeartMake(false)
	, m_CurPos({ 0,0 })
	, m_MoveDelayTime(0)
	, m_AttackTime(0)
	, nextIndex({ 5,0 })
	, prevIndex({ 5,0 })
	, m_firstContact(false)
{
	m_ActorTypeIndex = EActorTypeIndex::Monster;

}


Monster::~Monster()
{
}



void Monster::InitFSM()
{

}

void Monster::DebugRender()
{
	m_CalPos = ActorCalcPos(m_CurPos);
	swprintf_s(m_DebugText, L"(%d,%d) , CurPos X %f, Y %f", m_MonsterPosIndex.X, m_MonsterPosIndex.Y, m_CurPos.X, m_CurPos.Y);
	TextOut(GameTexMgr::BackDC(), static_cast<int>(m_CalPos.X), static_cast<int>(m_CalPos.Y), m_DebugText, lstrlen(m_DebugText));

	Rectangle(GameTexMgr::BackDC()
		, m_CalPos.IX() - 3
		, m_CalPos.IY() - 3
		, m_CalPos.IX() + 3
		, m_CalPos.IY() + 3
	);
}



void Monster::PlaySound(MonsterSound _SoundType)
{

}



void Monster::MonsterMove(CVector _CurPos, CVector _NextPos)
{
	Pos(CVector::Lerp(_CurPos, _NextPos, m_MoveTime * 3.0f));
}


bool Monster::Damaged(int _Attack)
{

	m_Health -= _Attack;
	PlaySound(Hit);

	if (m_Health <= 0)
	{
		State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
		Death();
		PlaySound(MonsterDeath);
		if (m_ActorTypeIndex != EActorTypeIndex::M_CoralDrum
			&& m_ActorTypeIndex != EActorTypeIndex::M_CoralHead
			&& m_ActorTypeIndex != EActorTypeIndex::M_CoralHorn
			&& m_ActorTypeIndex != EActorTypeIndex::M_CoralKeytar
			&& m_ActorTypeIndex != EActorTypeIndex::M_CoralString)
		{
			State()->CreateGold(m_MonsterPosIndex, m_Gold);
		}
		
		return true;
	}
	else if (m_bHeartMake == false)
	{
		
		m_bHeartMake = true;
		InitHeart({ (m_MonsterPosIndex.X * m_TileSize), (m_MonsterPosIndex.Y * m_TileSize) }, _Attack);
	}
	
	return false;

}

void Monster::ClearHeart()
{
	if (m_AllHeart.size() != 0)
	{
		m_AllHeart[0]->Death();
		m_AllHeart[1]->Death();
		m_AllHeart.clear();
	}

}

void Monster::InitHeart(CVector _TargetPos, int _Attack)
{
	_TargetPos.X -= 30;
	_TargetPos.Y -= 25;


	switch (m_ActorTypeIndex)
	{
	case EActorTypeIndex::M_minotaur:
		_TargetPos.X -= 25;
		_TargetPos.Y -= 30;
		break;
	case EActorTypeIndex::M_dragon:
		_TargetPos.X -= 30;
		_TargetPos.Y -= 35;
		break;
	default:
		break;
	}

	GameRender* NewHeart = nullptr;
	for (int i = 0; i < m_Health + _Attack; ++i)
	{
		NewHeart = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		_TargetPos.X += 20;


		NewHeart->SubPos(_TargetPos);
		NewHeart->SubSize({ 20 ,20 });
		NewHeart->Image(L"smallHeart.bmp");
		NewHeart->Index(1);
		m_AllHeart.push_back(NewHeart);
	}

	for (int i = 0; i < m_Health; ++i)
	{
		m_AllHeart[i]->Index(0);
	}
}


void Monster::InitDebug(CVector _TargetPos)
{
	m_CurPos = _TargetPos;
	MapIndex(m_CurPos);
	m_CalPos = ActorCalcPos(m_CurPos);
}


void Monster::InitAni(CVector _TargetPos)
{

}


void Monster::ChangeState(MonsterState _State)
{
	m_EState = _State;
}


void Monster::Update()
{
}