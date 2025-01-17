//#include "stdafx.h"
#include "Skeleton.h"


Skeleton::Skeleton(CVector _TargetPos) : m_Direction(true) , m_DelayTime (0)
{
	m_ActorTypeIndex = EActorTypeIndex::M_skeleton;

	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);

	m_Health = 1;
	m_Gold = 6;
	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	InitSound();

	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	m_StartPos = _TargetPos;
	m_EndPos = _TargetPos;

	m_MonsterPosIndex = m_Skeleton->SubPosIndeX();


	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}


Skeleton::~Skeleton()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}


void Skeleton::Update()
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
			m_firstContact = true;
		}

	}

	if (m_firstContact)
	{
		m_Skeleton->On();
	}
	else
	{
		m_Skeleton->Off();
		return;
	}

	Move();

	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}
}


void Skeleton::InitSound()
{
	SkelDeath = L"en_skel_death.mp3";
	SkelHeadLoss = L"en_skel_head_loss.mp3";
	SkelHit[0] = L"en_skel_hit.mp3";
	SkelHit[1] = L"en_skel_hit_01.mp3";
	SkelAttack = L"en_skel_attack_melee.mp3";
}


void Skeleton::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::Hit:
		if (m_HitCount > 1)
		{
			m_HitCount = 0;
		}
		GameSound::SoundPlayToOne(SkelHit[m_HitCount].c_str());
		++m_HitCount;
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(SkelDeath.c_str());
		break;
	case Monster::Attack:
		GameSound::SoundPlayToOne(SkelAttack.c_str());
		break;
	case Monster::Headloss:
		GameSound::SoundPlayToOne(SkelHeadLoss.c_str());
		break;
	default:
		break;
	}


}


void Skeleton::Move()
{
	m_MoveDelayTime += GameTime::DeltaTime();
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));
	m_DelayTime += GameTime::DeltaTime();

	if (m_CanAttack && m_DelayTime - 0.8f > 0)
	{
		ChangeState(Activity);
		m_CanAttack = false;
		m_DelayTime = 0;

	}
	else if (m_DelayTime - 0.8f > 0)
	{
		ChangeState(IDLE);
		m_CanAttack = true;
		m_DelayTime = 0;

	}

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

			if (m_CanAttack)
			{
				PlaySound(Attack);
				State()->GetPlayer()->Damaged(1);
			}
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



void Skeleton::ChangePos(CVector _Pos)
{
	m_Skeleton->SubPos(_Pos);

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



void Skeleton::InitAni(CVector _TargetPos)
{
	m_Skeleton = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Skeleton->CreateAni(IDLE, L"skeleton.bmp", 0, 3, true, 0.1f);
	m_Skeleton->CreateAni(Activity, L"skeleton.bmp", 4, 7, true, 0.1f);
	m_Skeleton->CreateAni(SkelLoss, L"skeleton.bmp", 9, 8, true, 0.1f);
	m_Skeleton->CreateAni(ShadowIDLE, L"skeleton.bmp", 9, 11, true, 0.1f);
	m_Skeleton->CreateAni(ShadowActivity, L"skeleton.bmp", 9, 15, true, 0.1f);
	m_Skeleton->SubPos(_TargetPos);
	m_Skeleton->SubSize(m_DefaultSize);

	//거리를 측정하고 State를 변경하게 하면 OK
	ChangeState(IDLE);
}


void Skeleton::InitFSM()
{

}

void Skeleton::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_Skeleton->ChangeAni(_State);
}