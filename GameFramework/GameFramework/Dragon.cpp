//#include "stdafx.h"
#include "Dragon.h"
#include <GameInput.h>

Dragon::Dragon(CVector _TargetPos)
	: m_HitCount(1)
	, m_AttackSpeed(1.0f)
	, m_AttackDelay(0)
	, m_AttackCheck(false)
	, m_ColCheck(false)
{
	nextIndex = CalcIndex(_TargetPos);
	prevIndex = CalcIndex(_TargetPos);
	m_Health = 5;
	m_Gold = 30;
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::M_dragon;
	//�巡���� �浹������ ���߱� ���� �ִϸ��̼ǰ� �浹ü ��ġ�� ������
	//������ 3,1�� ������ 3,2�� ������ ��, ���� ������ �ɰŰ��� ������
	m_CurPos = _TargetPos;
	m_NextPos = _TargetPos;
	m_StartPos = _TargetPos;
	m_EndPos = _TargetPos;

	//Ȥ�� ������ �߻��ϸ� �����ϱ�
	InitAni(_TargetPos);
	InitDebug(_TargetPos);
	InitSound();

	m_MonsterPosIndex = m_Dragon->SubPosIndeX();
	m_MapIndex = m_MonsterPosIndex;

	State()->m_MonsterMap.insert(std::map<__int64, Monster*>::value_type(
		m_MonsterPosIndex.Index,
		static_cast<Monster*>(this)));
}


Dragon::~Dragon()
{
	State()->m_MonsterMap.erase(m_MonsterPosIndex.Index);
	State()->MiniDie(true);
}


void Dragon::Update()
{

	if (m_AttackCheck)
	{
		if ((State()->GetPlayer()->PlayerIndex - m_MonsterPosIndex).IndexLen() < 2)
		{
			m_AttackDelay += GameTime::DeltaTime();
		}

		if (m_AttackDelay - m_AttackSpeed > 0)
		{
			State()->GetPlayer()->Damaged(4);
			PlaySound(Attack);
			m_AttackDelay = 0;
			m_AttackCheck = false;
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
			ChangeState(IDLE);
			m_firstContact = true;
		}
	}


	if (m_firstContact)
	{
		m_Dragon->On();
	}
	else
	{
		m_Dragon->Off();
		return;
	}

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

	Move();

	if (DustRenderer != nullptr && DustRenderer->IsCurAniEnd())
	{
		DustRenderer->Death();
		DustRenderer = nullptr;
	}
}


void Dragon::Move()
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

			nextMap.clear();
			return;
		}


		//���� ��ġ�� �÷��̾��� ���
		if (nextMap.size() != 0 && nextMap.begin()->first == 0)
		{
			m_NextPos = m_CurPos;
			nextIndex = m_MonsterPosIndex;
			nextMap.clear();
			m_MoveDelayTime = 0.0f;
			m_AttackCheck = true;
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
		PlaySound(Walk);

	}
}





void Dragon::ChangePos(CVector _Pos)
{
	m_Dragon->SubPos(_Pos);
	CVector subPosPure = _Pos;
	subPosPure.Y -= m_TileSizeHalf;
	m_Dragon->SubPosPure(subPosPure);

	if (m_AllHeart.size() != 0)
	{
		for (int i = 0; i < m_AllHeart.size(); ++i)
		{
			CVector tempPos = _Pos;
			tempPos.X += i * 20 - 30;
			tempPos.Y += -40 - m_TileSizeHalf;
			m_AllHeart[i]->SubPos(tempPos);
		}

	}


}


void Dragon::PlaySound(MonsterSound _SoundType)
{
	switch (_SoundType)
	{
	case Monster::Attack:
		GameSound::SoundPlayToOne(DragonAttack.c_str());
		break;
	case Monster::Hit:
		if (m_HitCount > 3)
		{
			m_HitCount = 1;
		}
		GameSound::SoundPlayToOne(DragonHit[m_HitCount].c_str());
		++m_HitCount;
		break;
	case Monster::MonsterDeath:
		GameSound::SoundPlayToOne(DragonDeath.c_str());
		break;
	case Monster::Walk:
		if (m_WalkCount > 1)
		{
			m_WalkCount = 0;
		}
		GameSound::SoundPlayToOne(DragonWalk[m_WalkCount].c_str());
		++m_WalkCount;
		break;
	default:
		break;
	}

}

void Dragon::InitSound()
{
	DragonHit[0] = L"en_dragon_hit.mp3";
	DragonHit[1] = L"en_dragon_hit_01.mp3";
	DragonHit[2] = L"en_dragon_hit_02.mp3";
	DragonHit[3] = L"en_dragon_hit_03.mp3";

	DragonWalk[0] = L"en_dragon_walk_01.mp3";
	DragonWalk[1] = L"en_dragon_walk_03.mp3";

	DragonDeath = L"en_dragon_death.mp3";
	DragonAttack = L"en_dragon_attack_melee.mp3";
}



void Dragon::InitAni(CVector _TargetPos)
{
	m_Dragon = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Dragon->CreateAni(IDLE, L"dragon_green.bmp", 0, 1, true, 0.5f);
	m_Dragon->CreateAni(ShadowIDLE, L"dragon_green.bmp", 2, 3, true, 0.5f);
	m_Dragon->SubPos(_TargetPos);
	CVector subPosPure = _TargetPos;
	subPosPure.Y -= m_TileSizeHalf;
	m_Dragon->SubPosPure(subPosPure);
	m_Dragon->SubSize(m_DefaultSize * 2.2f);

	ChangeState(ShadowIDLE);
}


void Dragon::InitFSM()
{

}

void Dragon::ChangeState(MonsterState _State)
{
	m_EState = _State;
	m_Dragon->ChangeAni(_State);
}

