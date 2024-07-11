//#include "stdafx.h"
#include "Minotaur.h"
#include "GameWall.h"

Minotaur::Minotaur(CVector _TargetPos)
	: m_bCharge(false)
	, m_Direction(true)
	, m_DelayTime(0)
	, m_HitCount(1)
	, m_AttackCheck(false)
	, m_AttackSpeed(0.5f)
	, m_AttackDelay(0)
{
	//�̳�Ÿ�츣���� �浹������ ���߱� ���� �ִϸ��̼ǰ� �浹ü ��ġ�� ������
	//������ 3,1�� ������ 3,2�� ������ ��, ���� ������ �ɰŰ��� ������
	//Ȥ�� ������ �߻��ϸ� �����ϱ�
	m_ActorTypeIndex = EActorTypeIndex::M_minotaur;
	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	InitSound();
	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	m_StartPos = _TargetPos;
	m_EndPos = _TargetPos;
	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);

	m_Health = 4;
	m_Gold = 30;
	m_MonsterPosIndex = m_Minotaur->SubPosIndeX();
	m_MapIndex = m_MonsterPosIndex;


	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}


Minotaur::~Minotaur()
{
	State()->MiniDie(true);
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
}


void Minotaur::Update()
{
	if (m_bHeartMake)
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
			if (!m_Delay)
			{
				ChangeState(IDLE);
			}
		}
	}

	if (m_firstContact)
	{
		m_Minotaur->On();
	}
	else
	{
		m_Minotaur->Off();
		return;
	}

	if (m_Delay)
	{
		m_DelayTime += GameTime::DeltaTime();

		if (m_DelayTime - 2.0f < 0)
		{
			return;
		}
		else
		{
			m_Delay = false;
			ChangeState(IDLE);
			m_DelayTime = 0;
		}

	}


	if (m_bCharge)
	{
		MoveFaster();
	}
	else
	{
		Move();
	}


	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}
}


void Minotaur::InitSound()
{
	MinotaurHit[0] = L"en_minotaur_hit.mp3";
	MinotaurHit[1] = L"en_minotaur_hit_01.mp3";
	MinotaurHit[2] = L"en_minotaur_hit_02.mp3";
	MinotaurAttack = L"en_minotaur_attack.mp3";
	MinotaurDeath = L"en_minotaur_death.mp3";
	MinotaurCharge = L"en_minotaur_charge.mp3";
	MinotaurCry = L"en_minotaur_cry.mp3";
	MinotaurWallImpact = L"en_minotaur_wallimpact.mp3";
}


void Minotaur::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::Attack:
		GameSound::SoundPlayToOne(MinotaurAttack.c_str());
		break;
	case Monster::Hit:
		if (m_HitCount > 3)
		{
			m_HitCount = 1;
		}
		GameSound::SoundPlayToOne(MinotaurHit[m_HitCount].c_str());
		++m_HitCount;
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(MinotaurDeath.c_str());
		break;
	case Monster::Charge:
		GameSound::SoundPlayToOne(MinotaurCharge.c_str());
		break;
	case Monster::Cry:
		GameSound::SoundPlayToOne(MinotaurCry.c_str());
		break;
	case Monster::WallCrush:
		GameSound::SoundPlayToOne(MinotaurWallImpact.c_str());
		break;
	default:
		break;
	}

}

void Minotaur::StopMino()
{

	m_StartPos = { m_Minotaur->SubPosIndeX().X * m_TileSize, m_Minotaur->SubPosIndeX().Y * m_TileSize };
	m_EndPos = m_StartPos;
	m_bCharge = false;
	m_Direction = true;
	m_Minotaur->ChangeAni(MinoDown);
	m_Delay = true;

	m_AttackTime = 0.0f;
}

void Minotaur::CheckAttack(float _AttackSPD)
{
	m_AttackTime += GameTime::DeltaTime();

	if (m_AttackTime - _AttackSPD > 0)
	{
		m_MonsterPosIndex = m_Minotaur->SubPosIndeX();

		m_MonsterFourPos[Left] = { m_MonsterPosIndex.X - 1, m_MonsterPosIndex.Y };
		m_MonsterFourPos[Up] = { m_MonsterPosIndex.X, m_MonsterPosIndex.Y - 1 };
		m_MonsterFourPos[Right] = { m_MonsterPosIndex.X + 1, m_MonsterPosIndex.Y };
		m_MonsterFourPos[Down] = { m_MonsterPosIndex.X , m_MonsterPosIndex.Y + 1 };

		m_LenArr[0] = State()->m_TileRender->Len(m_MonsterFourPos[Left]);
		m_LenArr[1] = State()->m_TileRender->Len(m_MonsterFourPos[Up]);
		m_LenArr[2] = State()->m_TileRender->Len(m_MonsterFourPos[Right]);
		m_LenArr[3] = State()->m_TileRender->Len(m_MonsterFourPos[Down]);


		switch (m_bCharge)
		{
		case true:
		{

			findIter = State()->m_WallActorMap.find((*ptrNextIndex).Index);
			EndIter = State()->m_WallActorMap.end();

			if (findIter != EndIter)
			{
				findIter->second->Death();
				State()->m_WallActorMap.erase(findIter);
				findIter = State()->m_BackWallActorMap.find((*ptrNextIndex).Index);
				findIter->second->Death();
				State()->m_BackWallActorMap.erase(findIter);
				StopMino();

			}
			else if ((*ptrNextIndex) == State()->GetPlayer()->PlayerIndex)
			{
				State()->GetPlayer()->Damaged(4);
				StopMino();
			}
		}
		break;
		case false:
		{
			if (nextIndex == State()->GetPlayer()->PlayerIndex)
			{
				m_StartPos = { m_Minotaur->SubPosIndeX().X * m_TileSize, m_Minotaur->SubPosIndeX().Y * m_TileSize };
				m_EndPos = m_StartPos;
				m_AttackTime = 0.0f;
			}
		}
		break;
		}

	}


	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}
}

void Minotaur::Move()
{
	m_MoveDelayTime += GameTime::DeltaTime();
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));

	if (m_MoveDelayTime - 0.6f > 0)
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
			return;
		}


		//���� ��ġ�� �÷��̾��� ���
		if (nextMap.size() != 0 && nextMap.begin()->first == 0)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			nextMap.clear();
			m_MoveDelayTime = 0.0f;
			m_bCharge = true;
			PlaySound(Charge);
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

		//�����Ÿ��� ���԰� X�� Y�� ������ �� �����϶�
		if (nextMap.size() != 0 && nextMap.begin()->first < 4)
		{
			if (State()->GetPlayer()->PlayerIndex.X - nextIndex.X == 0 || State()->GetPlayer()->PlayerIndex.Y - nextIndex.Y == 0)
			{
				m_bCharge = true;
				PlaySound(Charge);
			}
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


void Minotaur::MoveFaster()
{
	m_Minotaur->ChangeAni(Activity);
	ChangePos(CVector::Lerp(m_CurPos, m_NextPos, m_MoveDelayTime));

	m_MoveDelayTime += GameTime::DeltaTime();

	if (m_Direction)
	{
		{
			nextMap.clear();
			m_LenArr[0] = State()->m_TileRender->Len(m_MonsterFourPos[Left]);
			m_LenArr[1] = State()->m_TileRender->Len(m_MonsterFourPos[Up]);
			m_LenArr[2] = State()->m_TileRender->Len(m_MonsterFourPos[Right]);
			m_LenArr[3] = State()->m_TileRender->Len(m_MonsterFourPos[Down]);

			for (int i = 0; i < 4; ++i)
			{
				nextMap.insert(std::map<int, CVector::MapIndex>::value_type(m_LenArr[i], m_MonsterFourPos[i]));
			}

			nextIndex = nextMap.begin()->second;
			nextMap.clear();

		}

		prevIndex = nextIndex - State()->GetPlayer()->PlayerIndex;;

		if (prevIndex.X == 0 && prevIndex.Y == 0)
		{
			ptrNextIndex = &State()->GetPlayer()->PlayerIndex;
		}
		else if (prevIndex.X == 0)
		{
			if (prevIndex.Y > 0)
			{
				ptrNextIndex = &m_MonsterFourPos[Up];
			}
			else
			{
				ptrNextIndex = &m_MonsterFourPos[Down];
			}
		}
		else
		{
			if (prevIndex.X > 0)
			{
				ptrNextIndex = &m_MonsterFourPos[Left];
			}
			else
			{
				ptrNextIndex = &m_MonsterFourPos[Right];
			}
		}
		m_Direction = false;
	}

	if (m_MoveDelayTime - 0.4f > 0)
	{
		//�ϴ� �̵� �������� üũ
		//�� spirte�� �����ϴ� ��ġ�� ���� ��ġ.
		//SubPos�� Y�� 20��ŭ �ö� ����
		m_CurPos = { m_MonsterPosIndex.X * m_TileSize, m_MonsterPosIndex.Y * m_TileSize };

		m_MonsterFourPos[Left] = { m_MonsterPosIndex.X - 1, m_MonsterPosIndex.Y };
		m_MonsterFourPos[Up] = { m_MonsterPosIndex.X, m_MonsterPosIndex.Y - 1 };
		m_MonsterFourPos[Right] = { m_MonsterPosIndex.X + 1, m_MonsterPosIndex.Y };
		m_MonsterFourPos[Down] = { m_MonsterPosIndex.X , m_MonsterPosIndex.Y + 1 };

		//�ִܰŸ��� �������� ������ �Ǿ�����
		nextIndex = *ptrNextIndex;

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

			nextMap.clear();
			return;
		}


		//���� ��ġ�� �÷��̾��� ���
		if (nextIndex == State()->GetPlayer()->PlayerIndex)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			nextMap.clear();
			m_MoveDelayTime = 0.0f;
			m_bCharge = false;
			m_Delay = true;
			PlaySound(Attack);
			State()->GetPlayer()->Damaged(3);
			m_Minotaur->ChangeAni(MinoDown);
			m_Direction = true;
			return;
		}

		//���� ��ġ�� ���� ���
		findIter = State()->m_WallActorMap.find(nextIndex.Index);
		endIter = State()->m_WallActorMap.end();

		if (findIter != endIter)
		{
			GameWall* tempWall = static_cast<GameWall*>(findIter->second);
			if (tempWall->Def() < 10)
			{
				PlaySound(WallCrush);
				findIter->second->Death();
				State()->m_WallActorMap.erase(findIter);
				findIter = State()->m_BackWallActorMap.find(nextIndex.Index);
				findIter->second->Death();
				State()->m_BackWallActorMap.erase(findIter);
			}

			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			m_MoveDelayTime = 0.0f;
			nextMap.clear();
			m_Delay = true;
			m_bCharge = false;
			m_Minotaur->ChangeAni(MinoDown);
			m_Direction = true;
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
			nextMap.clear();
			m_Delay = true;
			m_bCharge = false;
			m_Minotaur->ChangeAni(MinoDown);
			m_Direction = true;
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




void Minotaur::ChangePos(CVector _Pos)
{
	m_Minotaur->SubPos(_Pos);
	CVector subPosPure = _Pos;
	subPosPure.Y -= m_TileSizeHalf;
	m_Minotaur->SubPosPure(subPosPure);

	if (m_AllHeart.size() != 0)
	{
		for (int i = 0; i < m_AllHeart.size(); ++i)
		{
			CVector tempPos = _Pos;
			tempPos.X += i * 20 - 35;
			tempPos.Y += -55;
			m_AllHeart[i]->SubPos(tempPos);
		}

	}


}


void Minotaur::InitAni(CVector _TargetPos)
{
	m_Minotaur = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Minotaur->CreateAni(IDLE, L"minotaur.bmp", 0, 3, true, 0.1f);
	m_Minotaur->CreateAni(Activity, L"minotaur.bmp", 4, 4, true, 0.1f);
	m_Minotaur->CreateAni(MinoDown, L"minotaur.bmp", 5, 8, true, 0.1f);
	m_Minotaur->CreateAni(ShadowIDLE, L"minotaur.bmp", 9, 12, true, 0.1f);
	m_Minotaur->CreateAni(ShadowActivity, L"minotaur.bmp", 13, 17, true, 0.1f);

	m_Minotaur->SubPos(_TargetPos);
	CVector subPosPure = _TargetPos;
	subPosPure.Y -= m_TileSizeHalf;
	m_Minotaur->SubPosPure(subPosPure);
	m_Minotaur->SubSize(m_DefaultSize * 2.0f);

	ChangeState(IDLE);
}


void Minotaur::InitFSM()
{

}

void Minotaur::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_Minotaur->ChangeAni(_State);
}
