//#include "stdafx.h"
#include "Monkey.h"
#include <GameSound.h>


Monkey::Monkey(CVector _TargetPos) : m_ChangeHealth(false), m_OnPlayer(false)
{
	m_ActorTypeIndex = EActorTypeIndex::M_monkey;

	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);

	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	InitSound();

	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	m_StartPos = _TargetPos;
	m_EndPos = _TargetPos;

	m_Health = 1;
	m_Gold = 4;
	m_MonsterPosIndex = m_Monkey->SubPosIndeX();
	m_MapIndex = m_MonsterPosIndex;

	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}


Monkey::~Monkey()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}


void Monkey::Update()
{
	if (m_Health <= 0)
	{
		Death();
		return;
	}


	if (m_Len != State()->m_TileRender->Len(m_MonsterPosIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MonsterPosIndex);

		if (m_Len > 9)
		{
			ChangeState(ShadowIDLE);
		}
		else if (!m_OnPlayer)
		{
			ChangeState(IDLE);
			m_firstContact = true;
		}
	}

	if (m_firstContact)
	{
		m_Monkey->On();
	}
	else
	{
		m_Monkey->Off();
		return;
	}

	if (!m_OnPlayer)
	{
		Move();
	}

	if (m_ChangeHealth)
	{
		for (int i = 0; i < 4; ++i)
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


	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}

}

bool Monkey::CheckIndex()
{
	//�ϴ� �̵� �������� üũ
		//�� spirte�� �����ϴ� ��ġ�� ���� ��ġ.
		//SubPos�� Y�� 20��ŭ �ö� ����
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
	//�ִܰŸ��� �������� ������ �Ǿ�����
	for (int i = 0; i < 4; ++i)
	{
		nextMap.insert(std::map<int, CVector::MapIndex>::value_type(m_LenArr[i], m_MonsterFourPos[i]));
	}

	nextIndex = nextMap.begin()->second;
	//�÷��̾�� ��ģ ���
	//���� �ε����� �̵�
	//���ʿ� Len�� ���� 0�϶��̹Ƿ� �׳� ����
	if (nextMap.size() == 1)
	{
		//m_CurPos = { prevIndex.X * m_TileSize, prevIndex.Y * m_TileSize };
		//m_NextPos = { prevIndex.X * m_TileSize, prevIndex.Y * m_TileSize };
		//ChangePos(m_NextPos);

		//findIter = State()->m_MonsterMap.find(m_MonsterPosIndex.Index);
		//endIter = State()->m_MonsterMap.end();
		//if (findIter != endIter)
		//{
		//	State()->m_MonsterMap.erase(findIter);
		//}
		//else
		//{
		//	//�ε��� ���� �����߻�. ���� �ε����� ������ �� �����Ƿ� �̵����� �ʽ��ϴ�.
		//	State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_MonsterPosIndex.Index, this));
		//	m_NextPos = m_CurPos;
		//	nextIndex = m_MonsterPosIndex;
		//	m_MoveDelayTime = 0.0f;
		//	nextMap.clear();
		//	return;
		//}
		//m_MonsterPosIndex = prevIndex;
		//nextIndex = m_MonsterPosIndex;
		//State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(prevIndex.Index, this));
		nextMap.clear();
		return false;
	}


	//���� ��ġ�� �÷��̾��� ���
	if (nextMap.size() != 0 && nextMap.begin()->first == 0)
	{
		m_NextPos = m_CurPos;
		nextIndex = m_MonsterPosIndex;
		nextMap.clear();
		m_MoveDelayTime = 0.0f;
		PlaySound(Grab);
		m_Monkey->ChangeAni(Activity);
		OnPlayer();
		return false;
	}

	//���� ��ġ�� ���� ���
	findIter = State()->m_WallActorMap.find(nextMap.begin()->second.Index);
	endIter = State()->m_WallActorMap.end();

	if (findIter != endIter)
	{
		m_NextPos = m_CurPos;
		nextIndex = m_MonsterPosIndex;
		m_MoveDelayTime = 0.0f;
		nextMap.clear();
		return false;
	}


	//���� ��ġ�� ���Ͱ� �ִ� ���
	findIter = State()->m_MonsterMap.find(nextIndex.Index);
	endIter = State()->m_MonsterMap.end();

	if (findIter != endIter)
	{
		
		m_NextPos = m_CurPos;
		nextIndex = m_MonsterPosIndex;
		m_MoveDelayTime = 0.0f;
		return false;
	}
	


	//�ʹ� �ָ� �ִ� ���
	if (nextMap.size() != 0 && nextMap.begin()->first > 7)
	{
		m_NextPos = m_CurPos;
		nextIndex = m_MonsterPosIndex;
		m_MoveDelayTime = 0.0f;
		nextMap.clear();
		return false;
	}

	findIter = State()->m_MonsterMap.find(nextIndex.Index);


	return true;
}

void Monkey::Move()
{
	m_MoveDelayTime += GameTime::DeltaTime();
	
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));

	if (m_MoveDelayTime - 0.6f > 0)
	{
	
		if (CheckIndex())
		{
			//��� �׽�Ʈ�� ����ߴٸ� ���� ��ġ �ε����� ���� �����ϰ� �̵���
			//�ε��� ����
			nextIndex = nextMap.begin()->second;
			auto tempIter = State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(nextIndex.Index, this));
			
			//�̵��Ϸ��� �ε��� ���� ����!
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
				//�ε��� ���� �����߻�. ���� �ε����� ������ �� �����Ƿ� �̵����� �ʽ��ϴ�.
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


			//�������

			m_NextPos = { m_MonsterPosIndex.X * m_TileSize, m_MonsterPosIndex.Y * m_TileSize };

			//�ð� �ʱ�ȭ
			//�̵��� �Ϸ�Ǿ��°�?
			m_CurPos = m_NextPos;
			m_MoveDelayTime = 0;
		}


	}
}


void Monkey::OnPlayer()
{
	m_OnPlayer = true;
	State()->GetPlayer()->OnMonkey(this);

	ChangePos({ State()->GetPlayer()->PlayerIndex.X * m_TileSize, State()->GetPlayer()->PlayerIndex.Y* m_TileSize });


	//��Ʈ ǥ�� ����
	if (!m_ChangeHealth)
	{
		m_bHeartMake = true;
		m_ChangeHealth = true;
		ClearHeart();
		m_Health = 4;
		InitHeart({ (m_MonsterPosIndex.X * m_TileSize), (m_MonsterPosIndex.Y * m_TileSize) }, 0);
	}

	if (m_AllHeart.size() != 0 && m_ChangeHealth)
	{
		for (int i = 0; i < m_AllHeart.size(); ++i)
		{
			CVector tempPos = { State()->GetPlayer()->PlayerIndex.X * m_TileSize + i* 20 - 30 , State()->GetPlayer()->PlayerIndex.Y * m_TileSize - 25 };
			m_AllHeart[i]->SubPos(tempPos);
		}

	}

	return;

}


void Monkey::ChangePos(CVector _Pos)
{
	m_Monkey->SubPos(_Pos);

	if (m_ChangeHealth)
	{
		if (m_AllHeart.size() != 0)
		{
			for (int i = 0; i < m_AllHeart.size(); ++i)
			{
				CVector tempPos = _Pos;
				tempPos.X += i * 20 - 30;
				tempPos.Y -= 25;
				m_AllHeart[i]->SubPos(tempPos);
			}

		}
	}
	else
	{
		if (m_AllHeart.size() != 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				CVector tempPos = _Pos;
				tempPos.X += i * 20 - 10;
				tempPos.Y -= 25;
				m_AllHeart[i]->SubPos(tempPos);
			}

		}
	}


}



void Monkey::InitAni(CVector _TargetPos)
{
	m_Monkey = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Monkey->CreateAni(IDLE, L"monkey.bmp", 0, 3, true, 0.1f);
	m_Monkey->CreateAni(Activity, L"monkey.bmp", 4, 5, true, 0.1f);
	m_Monkey->CreateAni(ShadowIDLE, L"monkey.bmp", 6, 9, true, 0.1f);
	m_Monkey->CreateAni(ShadowActivity, L"monkey.bmp", 10, 11, true, 0.1f);
	m_Monkey->SubPos(_TargetPos);
	m_Monkey->SubSize(m_DefaultSize);

	ChangeState(ShadowIDLE);
}



void Monkey::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::Hit:
		if (m_HitCount > 1)
		{
			m_HitCount = 0;
		}
		GameSound::SoundPlayToOne(MonkeyHit[m_HitCount].c_str());
		++m_HitCount;
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(MonkeyDeath.c_str());
		break;
	case Monster::Grab:
		GameSound::SoundPlayToOne(MonkeyGrab.c_str());
		break;
	default:
		break;
	}

}



void Monkey::InitSound()
{
	MonkeyHit[0] = L"en_monkey_hit_01.mp3";
	MonkeyHit[1] = L"en_monkey_hit_02.mp3";
	MonkeyDeath = L"en_monkey_death.mp3";
	MonkeyGrab = L"en_monkey_grab.mp3";

}

void Monkey::InitFSM()
{

}

void Monkey::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_Monkey->ChangeAni(_State);
}