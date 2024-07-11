//#include "stdafx.h"
#include "GreenSlime.h"

GreenSlime::GreenSlime(CVector _TargetPos) : m_GreenSlime(nullptr)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::M_greenSlime;
	m_Health = 1;
	m_Gold = 2;

	InitSound();
	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);

	m_MonsterPosIndex = m_GreenSlime->SubPosIndeX();
	m_MapIndex = m_MonsterPosIndex;

	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}


GreenSlime::~GreenSlime()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}

void GreenSlime::Update()
{
	if (m_Len != State()->m_TileRender->Len(m_MonsterPosIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MonsterPosIndex);

		if (m_Len > 9)
		{
			ChangeState(ShadowIDLE);
		}
		else
		{
			ChangeState(IDLE);
			m_firstContact = true;
		}
	}

	if (m_firstContact)
	{
		m_GreenSlime->On();
	}
	else
	{
		m_GreenSlime->Off();
	}
}



void GreenSlime::InitSound()
{
	BlueSlimeHit[0] = L"en_slime_hit.mp3";
	BlueSlimeHit[1] = L"en_slime_hit_01.mp3";
	BlueSlimeHit[2] = L"en_slime_hit_02.mp3";
	BlueSlimeDeath = L"en_slime_death.mp3";

}




void GreenSlime::PlaySound(MonsterSound _SoundType)
{

	switch (_SoundType)
	{
	case Monster::Hit:
		if (m_HitCount > 2)
		{
			m_HitCount = 0;
		}
		GameSound::SoundPlayToOne(BlueSlimeHit[m_HitCount].c_str());
		++m_HitCount;
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(BlueSlimeDeath.c_str());
		break;
	default:
		break;
	}

}


void GreenSlime::InitAni(CVector _TargetPos)
{
	m_GreenSlime = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_GreenSlime->CreateAni(IDLE, L"slime_green.bmp", 0, 3, true, 0.1f);
	m_GreenSlime->CreateAni(ShadowIDLE, L"slime_green.bmp", 4, 7, true, 0.1f);
	m_GreenSlime->SubPos(_TargetPos);
	m_GreenSlime->SubSize(m_DefaultSize);

	ChangeState(ShadowIDLE);
}


void GreenSlime::InitFSM()
{

}

void GreenSlime::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_GreenSlime->ChangeAni(_State);
}