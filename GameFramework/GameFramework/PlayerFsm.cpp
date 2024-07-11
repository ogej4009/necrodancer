//#include "stdafx.h"
#include "Player.h"
#include "GameTexMgr.h"
#include "Monster.h" 
#include "GameState.h"
#include "Weapon.h"
#include "Chest.h"



void Player::F_IDLE()
{

}

void Player::F_MOVE()
{

	switch (m_Dir)
	{
	case eDir::DIRR:
		m_CurPos = Pos();
		m_NextPos = m_CurPos + (CVector::RIGHT * m_TileSize);
		break;
	case eDir::DIRT:
		m_CurPos = Pos();
		m_NextPos = m_CurPos + (CVector::UP * m_TileSize);
		break;
	case eDir::DIRL:
		m_CurPos = Pos();
		m_NextPos = m_CurPos + (CVector::LEFT * m_TileSize);
		break;
	case eDir::DIRB:
		m_CurPos = Pos();
		m_NextPos = m_CurPos + (CVector::DOWN * m_TileSize);
		break;
	case eDir::DIRNONE:
	case eDir::DIRMAX:
		ASSERTMSG;
		break;
	default:
		break;
	}
	//초기 설정 이후 함수를 지나치면서 값이 바뀜
	m_EState = IDLE;
	m_PrevIndex = CalcIndex(m_CurPos);

	CheckIndex();
	WallCheck();
	WallOrderCheck();
	MonsterCheck();
	MonsterOrderCheck();
	StairCheck();

	if (m_bOnItem)
	{
		F_ITEM_ON_MOVE(m_ExtTItem);
		m_bOnItem = false;
	}

	if (m_CurPos == m_NextPos)
	{
		++m_AttackCount;
	}
	else
	{
		m_AttackCount = 0;
	}
	//m_Dir = eDir::DIRNONE;
}

void Player::F_BUY_ON_MOVE() {}
void Player::F_ITEM_ON_MOVE(GameActor* _Item)
{
	switch (m_ItemType)
	{
	case Player::InterActItem::Weapon:
	{
		Weapon* tempWeapon = static_cast<Weapon*>(_Item);
		m_WeaponType = static_cast<eWeapon>(tempWeapon->CurType((int)m_WeaponType));
	}
	break;
	case Player::InterActItem::Gold:
		break;
	case Player::InterActItem::BodyArmor:
	{
		m_Armor = true;

		m_PlayerLowBody->ChangeAni(OnArmor);
		m_PlayerBody->ChangeAni(OnArmor);
		m_PlayerHighBody->ChangeAni(OnArmor);

		m_PlayerHead->ChangeAni(IDLE);
		m_PlayerLowHead->ChangeAni(IDLE);
		m_PlayerHighHead->ChangeAni(IDLE);

	}
	break;
	case Player::InterActItem::Helmet:
	{
		m_Head = true;
	}
	break;
	default:
		ASSERTMSG;
		break;
	}

}
void Player::F_GOLD_ON_MOVE() {}
void Player::F_DOOR_ON_MOVE() {}
void Player::F_WALL_MOVE() {}
void Player::F_WALL_ATTACK() {}
void Player::F_WALL_DIAMOND_MOVE() {}
void Player::F_WALL_DIAMOND_ATTACK() {}
void Player::F_WALL_ITEM_MOVE() {}
void Player::F_WALL_ITEM_ATTACK() {}
void Player::F_MONSTER_MOVE() {}
void Player::F_MONSTER_ATTACK() {

	for (int i = 0; i < 3; ++i)
	{
		if (HitMonsterArr[i] != nullptr && HitMonsterArr[i]->ActorType() != EActorTypeIndex::Player)
		{
			if (HitMonsterArr[i]->ActorType() == EActorTypeIndex::Chest)
			{
				Chest* tempMonster = static_cast<Chest*>(HitMonsterArr[i]);
				tempMonster->OpenChest();
				HitMonsterArr[i] = nullptr;
				m_EState = IDLE;
				m_bOpenChest = true;
				continue;
			}

			if (HitMonsterArr[i] == nullptr)
			{
				continue;
			}

			Monster* tempMonster = static_cast<Monster*>(HitMonsterArr[i]);
			tempMonster->Damaged(1);
			m_EState = IDLE;
			HitMonsterArr[i] = nullptr;
			m_bOpenChest = false;


			if (m_AttackCount > 4)
			{
				m_AttackCount = 0;
			}

			CheckWeponDir();
			PlayWeaponAni();

			m_SoundIndex = CVector::RANDOM.RandomMinMax((m_AttackCount * 3), 4 + (m_AttackCount * 3) - 2);
			GameSound::SoundPlayToOne(PlayerMeleeSound[m_SoundIndex].c_str());

		}
	}


}
void Player::F_WET_FLOOR_MOVE() {}
void Player::F_THROW_ITEM() {}
void Player::F_WALL_LOCK_MOVE() {}
void Player::F_DOOR_LOCK_MOVE() {}


void Player::PlayWeaponAni()
{
	CVector Position = ActorCalcPos(Pos()) - CVector{ 480,270 };

	switch (m_WeaponDir)
	{
	case Player::Dagger_Left:
		PlayerSwipe[m_WeaponType]->On();
		Position.X -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Dagger_Left);
		break;
	case Player::Dagger_Up:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Dagger_Up);
		break;
	case Player::Dagger_Right:
		PlayerSwipe[m_WeaponType]->On();
		Position.X += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Dagger_Right);
		break;
	case Player::Dagger_Down:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Dagger_Down);
		break;
	case Player::Rapier_Left:
		PlayerSwipe[m_WeaponType]->On();
		Position.X -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Rapier_Left);
		break;
	case Player::Rapier_Up:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Rapier_Up);
		break;
	case Player::Rapier_Right:
		PlayerSwipe[m_WeaponType]->On();
		Position.X += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Rapier_Right);
		break;
	case Player::Rapier_Down:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->ChangeAni(Rapier_Down);
		break;
	case Player::LongSword_Left:
		PlayerSwipe[m_WeaponType]->On();
		Position.X -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize * 2,m_TileSize });
		PlayerSwipe[m_WeaponType]->ChangeAni(LongSword_Left);
		break;
	case Player::LongSword_Up:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize ,m_TileSize * 2 });
		PlayerSwipe[m_WeaponType]->ChangeAni(LongSword_Up);
		break;
	case Player::LongSword_Right:
		PlayerSwipe[m_WeaponType]->On();
		Position.X += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize * 2,m_TileSize });
		PlayerSwipe[m_WeaponType]->ChangeAni(LongSword_Right);
		break;
	case Player::LongSword_Down:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize ,m_TileSize * 2 });
		PlayerSwipe[m_WeaponType]->ChangeAni(LongSword_Down);
		break;
	case Player::BroadSword_Left:
		PlayerSwipe[m_WeaponType]->On();
		Position.X -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize ,m_TileSize * 3 });
		PlayerSwipe[m_WeaponType]->ChangeAni(BroadSword_Left);
		break;
	case Player::BroadSword_Up:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y -= m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize * 3 ,m_TileSize });
		PlayerSwipe[m_WeaponType]->ChangeAni(BroadSword_Up);
		break;
	case Player::BroadSword_Right:
		PlayerSwipe[m_WeaponType]->On();
		Position.X += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubPos(Position);
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize ,m_TileSize * 3 });
		PlayerSwipe[m_WeaponType]->ChangeAni(BroadSword_Right);
		break;
	case Player::BroadSword_Down:
		PlayerSwipe[m_WeaponType]->On();
		Position.Y += m_TileSize;
		PlayerSwipe[m_WeaponType]->SubSize({ m_TileSize * 3 ,m_TileSize });
		PlayerSwipe[m_WeaponType]->SubPos(Position);

		PlayerSwipe[m_WeaponType]->ChangeAni(BroadSword_Down);
		break;
	default:
		break;
	}
}


void Player::CheckWeponDir()
{
	switch (m_WeaponType)
	{
	case Player::Dagger:
		switch (m_Dir)
		{
		case eDir::DIRR:
			m_WeaponDir = Dagger_Right;
			break;
		case eDir::DIRT:
			m_WeaponDir = Dagger_Up;
			break;
		case eDir::DIRL:
			m_WeaponDir = Dagger_Left;
			break;
		case eDir::DIRB:
			m_WeaponDir = Dagger_Down;
			break;
		default:
			ASSERTMSG;
			break;
		}
		break;
	case Player::Rapier:
		switch (m_Dir)
		{
		case eDir::DIRR:
			m_WeaponDir = Rapier_Right;
			break;
		case eDir::DIRT:
			m_WeaponDir = Rapier_Up;
			break;
		case eDir::DIRL:
			m_WeaponDir = Rapier_Left;
			break;
		case eDir::DIRB:
			m_WeaponDir = Rapier_Down;
			break;
		default:
			ASSERTMSG;
			break;
		}
		break;
	case Player::LongSword:
		switch (m_Dir)
		{
		case eDir::DIRR:
			m_WeaponDir = LongSword_Right;
			break;
		case eDir::DIRT:
			m_WeaponDir = LongSword_Up;
			break;
		case eDir::DIRL:
			m_WeaponDir = LongSword_Left;
			break;
		case eDir::DIRB:
			m_WeaponDir = LongSword_Down;
			break;
		default:
			ASSERTMSG;
			break;
		}
		break;
	case Player::BroadSword:
		switch (m_Dir)
		{
		case eDir::DIRR:
			m_WeaponDir = BroadSword_Right;
			break;
		case eDir::DIRT:
			m_WeaponDir = BroadSword_Up;
			break;
		case eDir::DIRL:
			m_WeaponDir = BroadSword_Left;
			break;
		case eDir::DIRB:
			m_WeaponDir = BroadSword_Down;
			break;
		default:
			ASSERTMSG;
			break;
		}
		break;
	}
}