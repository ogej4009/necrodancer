//#include "stdafx.h"
#include "Zombie.h"
#include "GameState.h"


Zombie::Zombie(CVector _TargetPos) : m_Direction(true), m_DelayTime(0)
{
	m_ActorTypeIndex = EActorTypeIndex::M_zombie;

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

	m_Quardant = Quardant::One;
	m_PrevQuardant = Quardant::One;


	m_MonsterPosIndex = m_Zombie->SubPosIndeX();


	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}

Zombie::~Zombie()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}



void Zombie::Update()
{
	if (m_Len != State()->m_TileRender->Len(m_MonsterPosIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MonsterPosIndex);

		if (m_Len > 9)
		{
			switch (m_Quardant)
			{
			case Zombie::YPositive:
			case Zombie::XPositive:
			case Zombie::Zero:
			case Zombie::One:
			case Zombie::Two:
				m_Zombie->ChangeAni(ShadowFour);
				break;
			case Zombie::Three:
			case Zombie::XNegative:
				m_Zombie->ChangeAni(ShadowThree);
				break;
			case Zombie::Four:
			case Zombie::YNegative:
				m_Zombie->ChangeAni(ShadowOne);
				break;
			default:
				break;
			}
		}
		else
		{
			m_firstContact = true;
			switch (m_Quardant)
			{
			case Zombie::YPositive:
			case Zombie::XPositive:
			case Zombie::Zero:
			case Zombie::One:
			case Zombie::Two:
				m_Zombie->ChangeAni(Four);
				break;
			case Zombie::Three:
			case Zombie::XNegative:
				m_Zombie->ChangeAni(Three);
				break;
			case Zombie::Four:
			case Zombie::YNegative:
				m_Zombie->ChangeAni(One);
				break;
			default:
				break;
			}
		}

	}

	if (m_firstContact)
	{
		m_Zombie->On();
	}
	else
	{
		m_Zombie->Off();
		return;
	}

	Move();
	SetQuardant();

	if (m_PrevQuardant != m_Quardant)
	{
		switch (m_Quardant)
		{
		case Zombie::YPositive:
		case Zombie::XPositive:
		case Zombie::Zero:
		case Zombie::One:
		case Zombie::Two:
			m_Zombie->ChangeAni(Four);
			break;
		case Zombie::Three:
		case Zombie::XNegative:
			m_Zombie->ChangeAni(Three);
			break;
		case Zombie::Four:
		case Zombie::YNegative:
			m_Zombie->ChangeAni(One);
			break;
		default:
			break;
		}
	}

	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}
}


void Zombie::InitSound()
{
	ZombieDeath = L"en_zombie_death.mp3";
	ZombieAttack = L"en_zombie_attack.mp3";

}


void Zombie::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(ZombieDeath.c_str());
		break;
	case Monster::Attack:
		GameSound::SoundPlayToOne(ZombieAttack.c_str());
		break;
	default:
		break;
	}


}


void Zombie::Move()
{
	m_MoveDelayTime += GameTime::DeltaTime();
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));
	m_DelayTime += GameTime::DeltaTime();

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

		nextMap.clear();
		//�ִܰŸ��� �������� ������ �Ǿ�����
		for (int i = 0; i < 4; ++i)
		{
			nextMap.insert(std::map<int, CVector::MapIndex>::value_type(m_LenArr[i], m_MonsterFourPos[i]));
		}

		//�÷��̾�� ��ģ ���
		//���� �ε����� �̵�
		//���ʿ� Len�� ���� 0�϶��̹Ƿ� �׳� ����
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
				//�ε��� ���� �����߻�. ���� �ε����� ������ �� �����Ƿ� �̵����� �ʽ��ϴ�.
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


		//���� ��ġ�� �÷��̾��� ���
		if (nextMap.size() != 0 && nextMap.begin()->first == 0)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			nextMap.clear();
			m_MoveDelayTime = 0.0f;
			PlaySound(Attack);
			State()->GetPlayer()->Damaged(1);

			return;
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
			return;
		}


		//���� ��ġ�� ���Ͱ� �ִ� ���
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

		//�ʹ� �ָ� �ִ� ���
		if (nextMap.size() != 0 && nextMap.begin()->first > 7)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			nextMap.clear();
			return;
		}


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



void Zombie::ChangePos(CVector _Pos)
{
	m_Zombie->SubPos(_Pos);

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



void Zombie::InitAni(CVector _TargetPos)
{
	m_Zombie = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Zombie->CreateAni(Three, L"zombie.bmp", 8, 15, true, 0.1f);
	m_Zombie->CreateAni(Four, L"zombie.bmp", 16, 23, true, 0.1f);
	m_Zombie->CreateAni(One, L"zombie.bmp", 0, 7, true, 0.1f);
	m_Zombie->CreateAni(ShadowThree, L"zombie.bmp", 32, 39, true, 0.1f);
	m_Zombie->CreateAni(ShadowFour, L"zombie.bmp", 40, 47, true, 0.1f);
	m_Zombie->CreateAni(ShadowOne, L"zombie.bmp", 24, 31, true, 0.1f);

	m_Zombie->SubPos(_TargetPos);
	m_Zombie->SubSize(m_DefaultSize);

	//�Ÿ��� �����ϰ� State�� �����ϰ� �ϸ� OK
	m_Zombie->ChangeAni(One);
}



void Zombie::SetQuardant()
{
	m_PrevQuardant = m_Quardant;
	m_PlayerIndex = State()->GetPlayer()->PlayerIndex;

	/*if (m_MonsterPosIndex.X - m_PlayerIndex.X == 0)
	{
		if (m_MonsterPosIndex.Y - m_PlayerIndex.Y > 0)
		{
			m_Quardant = Quardant::YPositive;
		}
		else
		{
			m_Quardant = Quardant::YNegative;
		}
		return;
	}
	else if (m_MonsterPosIndex.Y - m_PlayerIndex.Y == 0)
	{
		if (m_MonsterPosIndex.X - m_PlayerIndex.X > 0)
		{
			m_Quardant = Quardant::XPositive;
		}
		else
		{
			m_Quardant = Quardant::XNegative;
		}
		return;
	}*/

	if (m_MonsterPosIndex.X - m_PlayerIndex.X > 0)
	{
		if (m_MonsterPosIndex.Y - m_PlayerIndex.Y > 0)
		{
			m_Quardant = Quardant::Four;
		}
		else
		{
			m_Quardant = Quardant::One;
		}
		return;
	}
	else //X < 0
	{
		if (m_MonsterPosIndex.Y - m_PlayerIndex.Y > 0)
		{
			m_Quardant = Quardant::Three;
		}
		else
		{
			m_Quardant = Quardant::Two;
		}
	}
}

void Zombie::InitFSM()
{

}

void Zombie::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_Zombie->ChangeAni(_State);
}