//#include "stdafx.h"
#include "Bat.h"


Bat::Bat(CVector _TargetPos)
{
	m_ActorTypeIndex = EActorTypeIndex::M_bat;

	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);
	m_Health = 1;
	m_Gold = 6;
	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	InitSound();

	m_MonsterPosIndex = m_Bat->SubPosIndeX();
	m_MapIndex = m_MonsterPosIndex;

	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}


Bat::~Bat()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}


void Bat::InitSound()
{
	BatHit = L"en_bat_hit.mp3";
	BatAttack = L"en_bat_attack.mp3";
	BatDeath = L"en_bat_death.mp3";
}



void Bat::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::Attack:
		GameSound::SoundPlayToOne(BatAttack.c_str());
		break;
	case Monster::Hit:
		GameSound::SoundPlayToOne(BatHit.c_str());
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(BatDeath.c_str());
		break;
	default:
		break;
	}

}


void Bat::Update()
{
	if (m_Len != State()->m_TileRender->Len(m_MonsterPosIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MonsterPosIndex);

		if (m_Len > 7)
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
		m_Bat->On();
	}
	else
	{
		m_Bat->Off();
		return;
	}

	Move();
}


void Bat::ChangePos(CVector _Pos)
{
	m_Bat->SubPos(_Pos);

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

void  Bat::Move()
{
	m_MoveDelayTime += GameTime::DeltaTime();
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));

	if (m_MoveDelayTime - 0.8f > 0)
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

		nextIndex = m_MonsterFourPos[CVector::RANDOM.RandomMinMax(0, 3)];


		//�÷��̾�� ��ģ ���
		//���� �ε����� �̵�
		//���ʿ� Len�� ���� 0�϶��̹Ƿ� �׳� ����
		if ((m_LenArr[0] == m_LenArr[1]) && (m_LenArr[1] == m_LenArr[2]) && (m_LenArr[2] == m_LenArr[3]))
		{
			if (m_LenArr[0] == 0)
			{
				return;
			}

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
				//�ε��� ���� �����߻�. ���� �ε����� ������ �� �����Ƿ� �̵����� �ʽ��ϴ�.
				State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_MonsterPosIndex.Index, this));
				m_NextPos = m_CurPos;
				nextIndex = m_MonsterPosIndex;
				m_MoveDelayTime = 0.0f;
				return;
			}

			m_MonsterPosIndex = prevIndex;
			nextIndex = m_MonsterPosIndex;
			State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(prevIndex.Index, this));

			return;
		}


		//���� ��ġ�� �÷��̾��� ���
		if (nextIndex == State()->GetPlayer()->PlayerIndex)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			State()->GetPlayer()->Damaged(1);
			m_MoveDelayTime = 0.0f;
			return;
		}

		//���� ��ġ�� ���� ���
		findIter = State()->m_WallActorMap.find(nextIndex.Index);
		endIter = State()->m_WallActorMap.end();

		if (findIter != endIter)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			return;
		}


		//���� ��ġ�� ���Ͱ� �ִ� ���
		findIter = State()->m_MonsterMap.find(nextIndex.Index);
		endIter = State()->m_MonsterMap.end();

		if (findIter != endIter)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			return;
		}


		//��� �׽�Ʈ�� ����ߴٸ� ���� ��ġ �ε����� ���� �����ϰ� �̵���
		//�ε��� ����
		findIter = State()->m_MonsterMap.find(m_MonsterPosIndex.Index);
		endIter = State()->m_MonsterMap.end();
		if (findIter != endIter)
		{
			State()->m_MonsterMap.erase(findIter);
		}
		else
		{
			//�ε��� ���� �����߻�. ���� �ε����� ������ �� �����Ƿ� �̵����� �ʽ��ϴ�.
			State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_MonsterPosIndex.Index, this));
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			return;
		}

		prevIndex = m_MonsterPosIndex;
		m_MonsterPosIndex = nextIndex;
		State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_MonsterPosIndex.Index, this));


		//�������

		m_NextPos = { m_MonsterPosIndex.X * m_TileSize, m_MonsterPosIndex.Y * m_TileSize };

		//�ð� �ʱ�ȭ
		//�̵��� �Ϸ�Ǿ��°�?
		m_CurPos = m_NextPos;
		m_MoveDelayTime = 0;
	}
}


void Bat::InitAni(CVector _TargetPos)
{
	m_Bat = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Bat->CreateAni(IDLE, L"bat.bmp", 0, 3, true);
	m_Bat->CreateAni(ShadowIDLE, L"bat.bmp", 4, 5, true);
	m_Bat->SubPos(_TargetPos);
	m_Bat->SubSize(m_DefaultSize);

	ChangeState(ShadowIDLE);
}


void Bat::InitFSM()
{

}

void Bat::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_Bat->ChangeAni(_State);
}