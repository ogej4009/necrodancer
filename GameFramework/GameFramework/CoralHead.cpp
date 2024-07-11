//#include "stdafx.h"
#include "CoralHead.h"

float CoralHead::GlobalTime;

CoralHead::CoralHead(CVector _TargetPos)
	: m_Direction(true)
	, m_DelayTime(0)
	, m_AttackCount(0)
	, m_SplashCount(0)
{
	m_ActorTypeIndex = EActorTypeIndex::M_CoralHead;

	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);

	m_Health = 5;
	m_Gold = 36;
	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	InitSound();

	m_InitPos = _TargetPos;
	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	m_StartPos = _TargetPos;
	m_EndPos = _TargetPos;
	m_CurHealth = (int)m_Health;

	m_MonsterPosIndex = m_CoralHead->SubPosIndeX();


	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));

}


CoralHead::~CoralHead()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
	State()->MiniDie(true);
}


void CoralHead::Update()
{
	if (m_bHeartMake)
	{
		for (int i = 0; i < 5; ++i)
		{
			if (i < m_Health)
			{
				m_AllHeart[i]->Index(0);
			}
			else
			{
				m_AllHeart[i]->Index(1);
			}
		}
	}

	if (m_Len != State()->m_TileRender->Len(m_MonsterPosIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MonsterPosIndex);

		if (m_Len > 9)
		{
			ChangeState(ShadowIDLE);
		}
		else
		{
			m_firstContact = true;
			ChangeState(IDLE);
		}

	}

	if (m_firstContact)
	{
		m_CoralHead->On();
		GlobalTime += GameTime::DeltaTime();
	}
	else
	{
		m_CoralHead->Off();
		return;
	}

	if (m_CurHealth != m_Health)
	{
		m_CurPos = m_InitPos;
		m_NextPos = m_InitPos;
		m_StartPos = m_InitPos;
		m_EndPos = m_InitPos;
		auto tempIter = State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(
			CalcIndex(m_InitPos).Index, static_cast<GameActor*>(this)));
		if (tempIter.second)
		{
			State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
			m_MonsterPosIndex = CalcIndex(m_InitPos);
			prevIndex = m_MonsterPosIndex;
			nextIndex = m_MonsterPosIndex;
			ChangePos(m_InitPos);
		}

		m_CurHealth = (int)m_Health;
		return;
	}


	if (GlobalTime - 25 > 0)
	{
		Move();
	}

	


	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}
}


void CoralHead::InitSound()
{
	CoralDeath = L"en_coralriff_death.mp3";
	CoralCry = L"en_coralriff_cry.mp3";

	CoralHit[0] = L"en_coralriff_hit_01.mp3";
	CoralHit[1] = L"en_coralriff_hit_02.mp3";
	CoralHit[2] = L"en_coralriff_hit_03.mp3";
	CoralHit[3] = L"en_coralriff_hit_04.mp3";

	CoralAttack[0] = L"en_coralriff_attack_mel_01.mp3";
	CoralAttack[1] = L"en_coralriff_attack_mel_02.mp3";
	CoralAttack[2] = L"en_coralriff_attack_mel_04.mp3";

	CoralSplash[0] = L"en_coralriff_attack_splash_01.mp3";
	CoralSplash[1] = L"en_coralriff_attack_splash_02.mp3";
	CoralSplash[2] = L"en_coralriff_attack_splash_03.mp3";
	CoralSplash[3] = L"en_coralriff_attack_splash_04.mp3";
}


void CoralHead::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::Hit:
		if (m_HitCount > 3)
		{
			m_HitCount = 0;
		}
		GameSound::SoundPlayToOne(CoralHit[m_HitCount].c_str());
		++m_HitCount;
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(CoralDeath.c_str());
		break;
	case Monster::Attack:
		if (m_AttackCount > 2)
		{
			m_AttackCount = 0;
		}
		GameSound::SoundPlayToOne(CoralAttack[m_AttackCount].c_str());
		++m_AttackCount;
		break;
	case Monster::Headloss:
		if (m_SplashCount > 3)
		{
			m_SplashCount = 0;
		}
		GameSound::SoundPlayToOne(CoralSplash[m_SplashCount].c_str());
		++m_SplashCount;
		break;
	default:
		break;
	}


}


void CoralHead::Move()
{
	m_MoveDelayTime += GameTime::DeltaTime();
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));
	m_DelayTime += GameTime::DeltaTime();


	if (m_MoveDelayTime - 0.8f > 0)
	{

		//일단 이동 가능한지 체크
		//내 spirte가 존재하는 위치가 현재 위치.
		//SubPos는 Y로 20만큼 올라가 있음
		m_CurPos = { m_MonsterPosIndex.X * m_TileSize, m_MonsterPosIndex.Y * m_TileSize };

		m_MonsterFourPos[Left] = { m_MonsterPosIndex.X - 1, m_MonsterPosIndex.Y };
		m_MonsterFourPos[Up] = { m_MonsterPosIndex.X, m_MonsterPosIndex.Y - 1 };
		m_MonsterFourPos[Right] = { m_MonsterPosIndex.X + 1, m_MonsterPosIndex.Y };
		m_MonsterFourPos[Down] = { m_MonsterPosIndex.X , m_MonsterPosIndex.Y + 1 };

		m_LenArr[0] = State()->m_TileRender->Len(m_MonsterFourPos[Left]);
		m_LenArr[1] = State()->m_TileRender->Len(m_MonsterFourPos[Up]);
		m_LenArr[2] = State()->m_TileRender->Len(m_MonsterFourPos[Right]);
		m_LenArr[3] = State()->m_TileRender->Len(m_MonsterFourPos[Down]);

		nextMap.clear();
		//최단거리는 앞쪽으로 나오게 되어있음
		for (int i = 0; i < 4; ++i)
		{
			nextMap.insert(std::map<int, CVector::MapIndex>::value_type(m_LenArr[i], m_MonsterFourPos[i]));
		}

		//플레이어와 겹친 경우
		//이전 인덱스로 이동
		//최초에 Len이 전부 0일때이므로 그냥 종료
		if (nextMap.size() == 1)
		{
			m_CurPos = { prevIndex.X * m_TileSize, prevIndex.Y * m_TileSize };
			m_NextPos = { prevIndex.X * m_TileSize, prevIndex.Y * m_TileSize };
			ChangePos(m_NextPos);

			findIter = State()->m_MonsterMap.find(m_MonsterPosIndex.Index);
			endIter = State()->m_MonsterMap.end();

			if (findIter != endIter)
			{
				State()->m_MonsterMap.erase(findIter);
			}
			else
			{
				//인덱스 점유 문제발생. 다음 인덱스를 점유할 수 없으므로 이동하지 않습니다.
				State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_MonsterPosIndex.Index, this));
				m_NextPos = m_CurPos;
				nextIndex = m_MonsterPosIndex;
				m_MoveDelayTime = 0.0f;
				nextMap.clear();
				return;
			}

			m_MonsterPosIndex = prevIndex;
			nextIndex = m_MonsterPosIndex;
			State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(prevIndex.Index, this));

			return;
		}


		//다음 위치가 플레이어인 경우
		if (nextMap.size() != 0 && nextMap.begin()->first == 0)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			nextMap.clear();
			m_MoveDelayTime = 0.0f;
			PlaySound(Attack);
			State()->GetPlayer()->Damaged(2);

			return;
		}

		//다음 위치에 벽인 경우
		findIter = State()->m_WallActorMap.find(nextMap.begin()->second.Index);
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
		findIter = State()->m_MonsterMap.find(nextMap.begin()->second.Index);
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
		nextIndex = nextMap.begin()->second;
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

		//물타일 만들기
		if (State()->m_WaterTileRender != nullptr && !State()->m_WaterTileRender->FindWaterTile(m_MonsterPosIndex))
		{
			State()->m_WaterTileRender->AddTile(m_MonsterPosIndex.X, m_MonsterPosIndex.Y);
		}


		//음악재생

		m_NextPos = { m_MonsterPosIndex.X * m_TileSize, m_MonsterPosIndex.Y * m_TileSize };

		//시간 초기화
		//이동이 완료되었는가?
		m_CurPos = m_NextPos;
		m_MoveDelayTime = 0;


	}
}



void CoralHead::ChangePos(CVector _Pos)
{
	m_CoralHead->SubPos(_Pos);
	CVector subPosPure = _Pos;
	subPosPure.Y -= m_TileSize;
	m_CoralHead->SubPosPure(subPosPure);

	if (m_AllHeart.size() != 0)
	{
		for (int i = 0; i < m_AllHeart.size(); ++i)
		{
			CVector tempPos = _Pos;
			tempPos.X += i * 20 - 40;
			tempPos.Y += -65;
			m_AllHeart[i]->SubPos(tempPos);
		}

	}

}



void CoralHead::InitAni(CVector _TargetPos)
{
	m_CoralHead = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_CoralHead->CreateAni(IDLE, L"coralriff_head.bmp", 0, 3, true, 0.1f);
	m_CoralHead->CreateAni(ShadowIDLE, L"coralriff_head.bmp", 4, 7, true, 0.1f);
	m_CoralHead->SubPos(_TargetPos);
	CVector subPosPure = _TargetPos;
	subPosPure.Y -= m_TileSize;
	m_CoralHead->SubPosPure(subPosPure);
	m_CoralHead->SubSize({ m_TileSize * 2 , m_TileSize * 2.5f });
	//거리를 측정하고 State를 변경하게 하면 OK
	ChangeState(IDLE);
}


void CoralHead::InitFSM()
{

}

void CoralHead::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_CoralHead->ChangeAni(_State);
}