//#include "stdafx.h"
#include "BlueSlime.h"


BlueSlime::BlueSlime(CVector _TargetPos) : m_bIsEnd(false)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::M_blueSlime;
	m_Health = 2;
	m_Gold = 4;
	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	m_StartPos = _TargetPos;
	m_EndPos = _TargetPos;

	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);
	int rowOrCol = CVector::RANDOM.RandomMinMax(0, 100);
	if (50 <= rowOrCol)
	{
		m_EndPos.Y += m_TileSize;
	}
	else
	{
		m_EndPos.X -= m_TileSize;
	}


	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	InitSound();

	m_MonsterPosIndex = m_BlueSlime->SubPosIndeX();
	m_MapIndex = m_MonsterPosIndex;

	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));


}


BlueSlime::~BlueSlime()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}


void BlueSlime::Update()
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
		m_BlueSlime->On();
	}
	else
	{
		m_BlueSlime->Off();
		return;
	}

	Move();
}


void BlueSlime::Move()
{

	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveTime * 4.0f));

	m_MoveDelayTime += GameTime::DeltaTime();

	if (m_MoveDelayTime - 1.2f > 0)
	{
		if (m_CurPos == m_StartPos)
		{
			m_NextPos = m_EndPos;
		}
		else if (m_CurPos == m_EndPos)
		{
			m_NextPos = m_StartPos;
		}

		nextIndex = CalcIndex(m_NextPos);
		m_LenArr[0] = State()->m_TileRender->Len(nextIndex);
		
		
		//다음 위치가 플레이어인 경우
		if (nextIndex == State()->GetPlayer()->PlayerIndex)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			nextMap.clear();
			m_MoveDelayTime = 0.0f;
			State()->GetPlayer()->Damaged(1);
			return;
		}

		//다음 위치에 벽인 경우
		findIter = State()->m_WallActorMap.find(nextIndex.Index);
		endIter = State()->m_WallActorMap.end();

		if (findIter != endIter)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			nextMap.clear();
			return;
		}


		//다음 위치에 몬스터가 있는 경우
		findIter = State()->m_MonsterMap.find(nextIndex.Index);
		endIter = State()->m_MonsterMap.end();

		if (findIter != endIter)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			nextMap.clear();
			return;
		}

		//너무 멀리 있는 경우
		if (nextMap.size() != 0 && nextMap.begin()->first > 7)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			nextMap.clear();
			return;
		}


		//모든 테스트를 통과했다면 다음 위치 인덱스를 먼저 선점하고 이동함
		//인덱스 점유
		auto tempIter = State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(nextIndex.Index, this));
		//이동하려는 인덱스 선점 성공!
		if (tempIter.second)
		{
			findIter = State()->m_MonsterMap.find(m_MonsterPosIndex.Index);
			endIter = State()->m_MonsterMap.end();
			if (findIter != endIter)
			{
				State()->m_MonsterMap.erase(findIter);
			}
		}
		else
		{
			//인덱스 점유 문제발생. 다음 인덱스를 점유할 수 없으므로 이동하지 않습니다.
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			nextMap.clear();
			return;
		}

		prevIndex = m_MonsterPosIndex;
		m_MonsterPosIndex = nextIndex;

		nextMap.clear();

		DustRenderer = CreateRender<AniRender>((int)m_ActorTypeIndex);
		DustRenderer->CreateAni(IDLE, L"jump_dirt.bmp", 0, 4, false, 0.1f);
		DustRenderer->SubSize(m_TileSize_Vector);
		DustRenderer->SubPos({ prevIndex.X * m_TileSize, prevIndex.Y * m_TileSize });
		DustRenderer->ChangeAni(IDLE);


		//음악재생

		m_NextPos = { m_MonsterPosIndex.X * m_TileSize, m_MonsterPosIndex.Y * m_TileSize };

		//시간 초기화
		//이동이 완료되었는가?
		m_CurPos = m_NextPos;
		m_MoveDelayTime = 0;

	}
}



void BlueSlime::ChangePos(CVector _Pos)
{
	//처음 생성될때 들어왔어
	//lerp결과값으로 이동해
	m_BlueSlime->SubPos(_Pos);

	if (m_AllHeart.size() != 0)
	{
		for (int i = 0; i < m_AllHeart.size(); ++i)
		{
			CVector tempPos = _Pos;
			tempPos.X += i * 20 - 10;
			tempPos.Y += -25;
			m_AllHeart[i]->SubPos(tempPos);
		}

	}

}



void BlueSlime::InitSound()
{
	BlueSlimeHit[0] = L"en_slime_hit.mp3";
	BlueSlimeHit[1] = L"en_slime_hit_01.mp3";
	BlueSlimeHit[2] = L"en_slime_hit_02.mp3";
	BlueSlimeAttack = L"en_slime_attack.mp3";
	BlueSlimeDeath = L"en_slime_death.mp3";

}




void BlueSlime::PlaySound(MonsterSound _SoundType)
{

	switch (_SoundType)
	{
	case Monster::Attack:
		GameSound::SoundPlayToOne(BlueSlimeAttack.c_str());
		break;
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



void BlueSlime::InitAni(CVector _TargetPos)
{
	m_BlueSlime = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_BlueSlime->CreateAni(IDLE, L"slime_blue.bmp", 0, 7, true, 0.07f);
	m_BlueSlime->CreateAni(ShadowIDLE, L"slime_blue.bmp", 8, 14, true, 0.07f);
	m_BlueSlime->SubPos(_TargetPos);
	m_BlueSlime->SubSize(m_DefaultSize);

	ChangeState(ShadowIDLE);
}


void BlueSlime::InitFSM()
{

}

void BlueSlime::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_BlueSlime->ChangeAni(_State);
}