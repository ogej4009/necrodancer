//#include "stdafx.h"
#include "Player.h"
#include <GameWin.h>
#include <GameMath.h>
#include <GameInput.h>
#include <GameTime.h>
#include <GameTexMgr.h>
#include "GameState.h"
#include "Bullet.h"
#include "GameRender.h"
#include "GameState.h"
#include "AniRender.h"
#include "GameCol.h"
#include "ValueData.h"
#include "GameMap.h"
#include "Monster.h"
#include "Weapon.h"
#include "Monster.h"
#include "GameDoor.h"
#include "GameWall.h"
#include "Monkey.h"
#include "ShopItem.h"
#include "ShopKeeper.h"
#include "TileRender.h"
#include "CoralHead.h"



float Player::m_JumpTimer = 0.0f;
float Player::m_FPSUpdate = 1.0f;
int Player::m_SoundIndex;

CVector::MapIndex Player::PlayerIndex;
CVector::MapIndex Player::nextPlayerIndex;

 int Player::m_StaticPlayerWeaponType = 0;
 int Player::m_StaticPlayerGold = 100;
int Player::m_StaticPlayerHealth = 10;
bool Player::m_StaticHead = false;
 bool Player::m_StaticArmor = false;


//플레이어는 state에 m_player로 받아놔져있따.
Player::Player()
	: m_NextPos({ 0.0f, 0.0f })
	, m_CurPos({ 0.0f, 0.0f })
	, m_bKeyInput(false)
	, m_bReadyInput(false)
	, m_bCanMove(false)
	, m_bIsMove(false)
	, m_bIsJump(false)
	, m_bOnStair(false)
	, m_PlayerHealth(10)
	, m_PlayerGold(0)
	, m_PlayerDiamond(0)
	, m_EState(IDLE)
	, m_PrevDir(eDir::DIRNONE)
	, m_Dir(eDir::DIRNONE)
	, Xindex(0)
	, Yindex(0)
	, m_WeaponType(Dagger)
	, m_bOnItem(false)
	, m_Armor(false)
	, m_Head(false)
	, m_bPressUp(false)
	, m_bPressDown(false)
	, m_bPressRight(false)
	, m_bPressLeft(false)
	, m_PlayerShovelLev(1)
	, m_bWaterTileCheck(false)
	, m_WaterSound(false)
	, m_WaterJump(0)
	, m_bOpenChest(false)

{
	
	m_PlayerGold = 100;
	Pos({ 0,0 });
	//상속
	m_ActorTypeIndex = EActorTypeIndex::Player;
	HitMonsterArr[0] = nullptr;
	HitMonsterArr[1] = nullptr;
	HitMonsterArr[2] = nullptr;

	InitAni();
	InitFSM();
	InitPlayerSound();

	MoveTime = 0.0f;
	CVector::MapIndex temp = { 0,0 };
	State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(temp.Index, this));
}

Player::~Player()
{
}

void Player::CheckItem()
{
	m_PlayerGold = m_StaticPlayerGold;
	m_PlayerHealth = m_StaticPlayerHealth;
	m_Head = m_StaticHead;
	m_Armor = m_StaticArmor;
	m_WeaponType = (eWeapon)m_StaticPlayerWeaponType;

	if (m_Armor)
	{
		m_PlayerLowBody->ChangeAni(OnArmor);
		m_PlayerBody->ChangeAni(OnArmor);
		m_PlayerHighBody->ChangeAni(OnArmor);

		m_PlayerHead->ChangeAni(IDLE);
		m_PlayerLowHead->ChangeAni(IDLE);
		m_PlayerHighHead->ChangeAni(IDLE);
	}
}

void Player::ChangeGlobal()
{
	m_StaticPlayerGold = m_PlayerGold;
	m_StaticPlayerHealth = m_PlayerHealth;
	m_StaticHead = m_Head;
	m_StaticArmor = m_Armor;
	m_StaticPlayerWeaponType = (int)m_WeaponType;
}


void Player::PlayerMove()
{
	if (m_JumpTimer < 0.25f && true == m_bIsJump)
	{
		Pos(CVector::Lerp(m_CurPos, m_NextPos + CVector::UP * (m_TileSize - 10.0f - m_WaterJump), GameState::m_CamTimer * 3.0f));
	}
	else
	{
		Pos(CVector::Lerp(m_CurPos, m_NextPos, GameState::m_CamTimer * 3.0f));
	}

	if (Pos() != m_NextPos)
	{
		m_JumpTimer += GameTime::DeltaTime();
	}
}


void Player::InitPlayerSound()
{

	PlayerStateSound[0] = L"sfx_player_death_ST.mp3";
	PlayerStateSound[1] = L"sfx_player_hit_ST.mp3";

	PlayerMeleeSound[0] = L"vo_cad_melee_1_01.mp3";
	PlayerMeleeSound[1] = L"vo_cad_melee_1_02.mp3";
	PlayerMeleeSound[2] = L"vo_cad_melee_1_03.mp3";
	PlayerMeleeSound[3] = L"vo_cad_melee_1_04.mp3";
	PlayerMeleeSound[4] = L"vo_cad_melee_2_01.mp3";
	PlayerMeleeSound[5] = L"vo_cad_melee_2_02.mp3";
	PlayerMeleeSound[6] = L"vo_cad_melee_2_03.mp3";
	PlayerMeleeSound[7] = L"vo_cad_melee_3_01.mp3";
	PlayerMeleeSound[8] = L"vo_cad_melee_3_02.mp3";
	PlayerMeleeSound[9] = L"vo_cad_melee_3_03.mp3";
	PlayerMeleeSound[10] = L"vo_cad_melee_4_01.mp3";
	PlayerMeleeSound[11] = L"vo_cad_melee_4_02.mp3";
	PlayerMeleeSound[12] = L"vo_cad_melee_4_03.mp3";
	PlayerMeleeSound[13] = L"vo_cad_melee_4_04.mp3";
	PlayerMeleeSound[14] = L"vo_cad_melee_4_05.mp3";

	PlayerDigSound[0] = L"mov_dig_fail.mp3";
	PlayerDigSound[1] = L"mov_dig_dirt.mp3";
	PlayerDigSound[2] = L"mov_dig_brick.mp3";
	PlayerDigSound[3] = L"mov_dig_stone.mp3";

	PlayerWaterInOut[0] = L"mov_water_in.mp3";
	PlayerWaterInOut[1] = L"mov_water_out.mp3";

}


void Player::CheckCurIndex()
{
	m_CurXIndex = static_cast<int>(std::round(Pos().IX() / m_TileSize));
	m_CurYIndex = static_cast<int>(std::round(Pos().IY() / m_TileSize));
	m_MapIndex = CVector::MapIndex({ m_CurXIndex,m_CurYIndex });
	PlayerIndex = m_MapIndex;
	nextPlayerIndex = { Xindex , Yindex };
}

void Player::Update()
{
	// 가장 중요하다고 생각하는 핵심요소
	// 삭제.
	// 플레이 도중 삭제

	m_bCanMove = false;
	PlayerMove();

	if (PlayerSwipe[m_WeaponType]->IsCurAniEnd())
	{
		PlayerSwipe[m_WeaponType]->Off();
	}

	if (m_Shovel != nullptr && m_Shovel->IsCurAniEnd())
	{
		m_Shovel->Off();
	}


	if (EnemySwipe->IsCurAniEnd())
	{
		EnemySwipe->Off();
	}

	if (m_OnMonkey)
	{

		m_PlayerLowBody->On();
		m_PlayerLowHead->On();

		m_PlayerHead->Off();
		m_PlayerBody->Off();

		m_PlayerHighBody->Off();
		m_PlayerHighHead->Off();

		if (GameInput::Down(L"MOVEL"))
		{
			m_OnMonkey = !m_ptrMonkey->Damaged(1);
			return;
		}

		if (GameInput::Down(L"MOVER"))
		{
			m_OnMonkey = !m_ptrMonkey->Damaged(1);
			return;
		}

		if (GameInput::Down(L"MOVET"))
		{
			m_OnMonkey = !m_ptrMonkey->Damaged(1);
			return;
		}

		if (GameInput::Down(L"MOVEB"))
		{
			m_OnMonkey = !m_ptrMonkey->Damaged(1);
			return;
		}

	}
	//CheckCurIndex();
	

	if (State()->m_WaterTileRender != nullptr && State()->m_WaterTileRender->FindWaterTile(PlayerIndex))
	{
		m_WaterJump = 15;
		if (!m_WaterSound)
		{
			GameSound::SoundPlayToOne(PlayerWaterInOut[0].c_str());
			m_WaterSound = true;
		}
		
		float delay = 0;

		if (GameInput::Down(L"MOVEL"))
		{
			m_bIsJump = true;
			m_JumpTimer = 0.17f;
			State()->m_WaterTileRender->DeleteWaterTile(PlayerIndex);
			GameSound::SoundPlayToOne(PlayerWaterInOut[1].c_str());
			return;
		}

		if (GameInput::Down(L"MOVER"))
		{
			m_bIsJump = true;
			m_JumpTimer = 0.17f;
			State()->m_WaterTileRender->DeleteWaterTile(PlayerIndex);
			GameSound::SoundPlayToOne(PlayerWaterInOut[1].c_str());
			return;
		}

		if (GameInput::Down(L"MOVET"))
		{
			m_bIsJump = true;
			m_JumpTimer = 0.17f;
			State()->m_WaterTileRender->DeleteWaterTile(PlayerIndex);
			GameSound::SoundPlayToOne(PlayerWaterInOut[1].c_str());
			return;
		}

		if (GameInput::Down(L"MOVEB"))
		{
			m_bIsJump = true;
			m_JumpTimer = 0.17f;
			State()->m_WaterTileRender->DeleteWaterTile(PlayerIndex);
			GameSound::SoundPlayToOne(PlayerWaterInOut[1].c_str());
			return;
		}

		return;
	}
	else
	{
		m_WaterJump = 0;
		m_WaterSound = false;
	}


	//이동이 끝날때까지 이동체크 안함
	if (GameState::m_CamTimer - 0.34f < 0)
	{
		if (m_EState == PLAYERSTATE::MONSTER_ATTACK && m_WeaponType == Rapier)
		{
			F_MONSTER_ATTACK();
		}


		return;
	}

	if (m_Damaged->IsOn())
	{
		m_DamagedTimer += GameTime::DeltaTime();

		if (m_DamagedTimer - 0.3f > 0)
		{
			m_Damaged->Off();
			m_DamagedTimer = 0.0f;
		}

	}
	//PlayerMeleeSound[m_SoundIndex]->Position(0);


	DirCheck();
	DirChangeCheck();
	CheckCurIndex();

	switch (m_EState)
	{
	case Player::IDLE:
		F_IDLE();
		break;
	case Player::MOVE:
		F_MOVE();
		break;
	case Player::BUY_ON_MOVE:
		F_BUY_ON_MOVE();
		break;
	case Player::ITEM_ON_MOVE:
		F_ITEM_ON_MOVE(m_ExtTItem);
		break;
	case Player::GOLD_ON_MOVE:
		F_GOLD_ON_MOVE();
		break;
	case Player::DOOR_ON_MOVE:
		F_DOOR_ON_MOVE();
		break;
	case Player::WALL_MOVE:
		F_WALL_MOVE();
		break;
	case Player::WALL_ATTACK:
		F_WALL_ATTACK();
		break;
	case Player::WALL_DIAMOND_MOVE:
		F_WALL_DIAMOND_MOVE();
		break;
	case Player::WALL_DIAMOND_ATTACK:
		F_WALL_DIAMOND_ATTACK();
		break;
	case Player::WALL_ITEM_MOVE:
		F_WALL_ITEM_MOVE();
		break;
	case Player::WALL_ITEM_ATTACK:
		F_WALL_ITEM_ATTACK();
		break;
	case Player::MONSTER_MOVE:
		F_MONSTER_MOVE();
		break;
	case Player::MONSTER_ATTACK:
		F_MONSTER_ATTACK();
		break;
	case Player::WET_FLOOR_MOVE:
		F_WET_FLOOR_MOVE();
		break;
	case Player::THROW_ITEM:
		F_THROW_ITEM();
		break;
	case Player::WALL_LOCK_MOVE:
		F_WALL_LOCK_MOVE();
		break;
	case Player::DOOR_LOCK_MOVE:
		F_DOOR_LOCK_MOVE();
		break;
	case Player::STATE_MAX:
		ASSERTMSG;
		break;
	default:
		break;
	}



	if (true == m_bKeyInput)
	{
		m_JumpTimer = 0.0f;
	}

	if (m_bCheckMonster && m_bCheckWall)
	{
		m_bCanMove = true;
		/*auto findIter = State()->m_MonsterMap.find(PlayerIndex.Index);
		auto endIter = State()->m_MonsterMap.end();
		if (findIter != endIter)
		{
			State()->m_MonsterMap.erase(findIter);
		}*/

	}
	else
	{
		m_bPressDown = false;
		m_bPressRight = false;
		m_bPressLeft = false;
		m_bPressUp = false;
	}

}

void Player::CheckIndex()
{
	Xindex = static_cast<int>(m_NextPos.X / m_TileSize);
	Yindex = static_cast<int>(m_NextPos.Y / m_TileSize);

	//플레이어 좌표 찾기
	m_PlayerIndexArr[0] = { Xindex - 2, Yindex - 2 };
	m_PlayerIndexArr[1] = { Xindex - 1, Yindex - 2 };
	m_PlayerIndexArr[2] = { Xindex    , Yindex - 2 };
	m_PlayerIndexArr[3] = { Xindex + 1, Yindex - 2 };
	m_PlayerIndexArr[4] = { Xindex + 2, Yindex - 2 };
	m_PlayerIndexArr[5] = { Xindex - 2, Yindex - 1 };
	m_PlayerIndexArr[6] = { Xindex - 1, Yindex - 1 };
	m_PlayerIndexArr[7] = { Xindex    , Yindex - 1 };
	m_PlayerIndexArr[8] = { Xindex + 1, Yindex - 1 };
	m_PlayerIndexArr[9] = { Xindex + 2, Yindex - 1 };
	m_PlayerIndexArr[10] = { Xindex - 2, Yindex };
	m_PlayerIndexArr[11] = { Xindex - 1, Yindex };
	m_PlayerIndexArr[12] = { Xindex    , Yindex };
	m_PlayerIndexArr[13] = { Xindex + 1, Yindex };
	m_PlayerIndexArr[14] = { Xindex + 2, Yindex };
	m_PlayerIndexArr[15] = { Xindex - 2, Yindex + 1 };
	m_PlayerIndexArr[16] = { Xindex - 1, Yindex + 1 };
	m_PlayerIndexArr[17] = { Xindex    , Yindex + 1 };
	m_PlayerIndexArr[18] = { Xindex + 1, Yindex + 1 };
	m_PlayerIndexArr[19] = { Xindex + 2, Yindex + 1 };
	m_PlayerIndexArr[20] = { Xindex - 2, Yindex + 2 };
	m_PlayerIndexArr[21] = { Xindex - 1, Yindex + 2 };
	m_PlayerIndexArr[22] = { Xindex    , Yindex + 2 };
	m_PlayerIndexArr[23] = { Xindex + 1, Yindex + 2 };
	m_PlayerIndexArr[24] = { Xindex + 2, Yindex + 2 };

}



void Player::WallCheck()
{

	GameActor::FindIter = State()->m_WallActorMap.find(m_PlayerIndexArr[12].Index);
	GameActor::EndIter = State()->m_WallActorMap.end();
	if (GameActor::FindIter != EndIter)
	{
		if (FindIter->second->ActorType() == EActorTypeIndex::Door)
		{
			GameDoor * tempDoor = static_cast<GameDoor*>(FindIter->second);
			tempDoor->OpenDoor();
		}
		else
		{
			GameWall* newWall = static_cast<GameWall*>(FindIter->second);
			if (newWall->Def() <= m_PlayerShovelLev)
			{
				FindIter->second->Death();
				State()->m_WallActorMap.erase(FindIter);
				FindIter = State()->m_BackWallActorMap.find(m_PlayerIndexArr[12].Index);
				FindIter->second->Death();
				FindIter = State()->m_BackWallActorMap.erase(FindIter);

				GameSound::SoundPlay(PlayerDigSound[m_PlayerShovelLev].c_str());
				m_Shovel->SubPos(m_NextPos - m_CurPos);
				m_Shovel->On();

			}
			else
			{
				GameSound::SoundPlay(PlayerDigSound[0].c_str());
				m_Shovel->SubPos(m_NextPos - m_CurPos);
				m_Shovel->On();
			}
			
			//맵 제작용 임시주석
			m_NextPos = m_CurPos;
			m_bCheckWall = false;
			m_bIsJump = false;
		}
	}
	else
	{
		m_bCheckWall = true;
	}
}

void Player::WallOrderCheck()
{
	for (int i = 16; i < 19; ++i)
	{
		GameActor::FindIter = State()->m_WallActorMap.find(m_PlayerIndexArr[i].Index);
		GameActor::EndIter = State()->m_WallActorMap.end();
		if (GameActor::FindIter != EndIter)
		{
			m_PlayerLowHead->On();
			m_PlayerLowBody->On();
			m_PlayerHead->Off();
			m_PlayerBody->Off();
			m_PlayerHighHead->Off();
			m_PlayerHighBody->Off();
			return;
		}
	}
	for (int i = 6; i < 9; ++i)
	{
		GameActor::FindIter = State()->m_WallActorMap.find(m_PlayerIndexArr[i].Index);
		GameActor::EndIter = State()->m_WallActorMap.end();
		if (GameActor::FindIter != EndIter)
		{
			m_PlayerLowHead->Off();
			m_PlayerLowBody->Off();
			m_PlayerHead->Off();
			m_PlayerBody->Off();
			m_PlayerHighHead->On();
			m_PlayerHighBody->On();
			return;
		}
	}

}

void Player::StairCheck()
{
	GameActor::FindIter = State()->m_InterObjMap.find(m_PlayerIndexArr[12].Index);
	GameActor::EndIter = State()->m_InterObjMap.end();

	if (FindIter != EndIter && EActorTypeIndex::Stair == FindIter->second->ActorType() ||
		FindIter != EndIter && EActorTypeIndex::StairLock == FindIter->second->ActorType())
	{
		m_bOnStair = true;
		return;
	}

	m_bOnStair = false;

}

void Player::MonsterCheck()
{
	//플레이어도 몬스터 맵에 넣어버리자 :: 절대로 겹칠수가 없도록


	std::map<__int64, GameActor*>::iterator FindIter;
	std::map<__int64, GameActor*>::iterator EndIter = State()->m_MonsterMap.end();
	CVector tempNextPos = m_NextPos;

	FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[12].Index);
	if (FindIter != EndIter && FindIter->second->ActorType() != EActorTypeIndex::Player)
	{
		m_EState = PLAYERSTATE::MONSTER_ATTACK;
		HitMonsterArr[0] = FindIter->second;
		m_NextPos = m_CurPos;
		m_bCheckMonster = false;
		m_bIsJump = false;
	}



	switch (m_WeaponType)
	{
	case Player::Dagger:
		break;
	case Player::Rapier:
		switch (m_Dir)
		{
		case eDir::DIRR:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[13].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;
			}
			break;
		case eDir::DIRT:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[7].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;
			}
			break;
		case eDir::DIRL:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[11].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;
			}
			break;
		case eDir::DIRB:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[17].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;
			}
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
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[13].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			break;
		case eDir::DIRT:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[7].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			break;
		case eDir::DIRL:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[11].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			break;
		case eDir::DIRB:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[17].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			break;
		default:
			ASSERTMSG;
			break;
		}
		break;
	case Player::BroadSword:
		switch (m_Dir)
		{
		case eDir::DIRL:
		case eDir::DIRR:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[7].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[17].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[2] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			break;
		case eDir::DIRB:
		case eDir::DIRT:
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[11].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[1] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[13].Index);
			if (FindIter != EndIter)
			{
				m_EState = PLAYERSTATE::MONSTER_ATTACK;
				HitMonsterArr[2] = FindIter->second;

				m_NextPos = m_CurPos;
				m_bCanMove = false;
				m_bIsJump = false;
				m_bCheckMonster = false;
			}
			break;
		default:
			ASSERTMSG;
			break;
		}
		break;
	default:
		break;
	}


	if (m_bCheckWall && HitMonsterArr[0] == nullptr && HitMonsterArr[1] == nullptr && HitMonsterArr[2] == nullptr)
	{
		auto tempIter = State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_PlayerIndexArr[12].Index, this));
		if (tempIter.second)
		{
			m_bCheckMonster = true;
			State()->m_MonsterMap.erase(m_PrevIndex.Index);
		}
	}

}

void Player::MonsterOrderCheck()
{
	for (int i = 16; i < 19; ++i)
	{
		auto FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[i].Index);
		auto EndIter = State()->m_MonsterMap.end();
		if (FindIter != EndIter)
		{
			m_PlayerLowHead->On();
			m_PlayerLowBody->On();
			m_PlayerHead->Off();
			m_PlayerBody->Off();
			m_PlayerHighHead->Off();
			m_PlayerHighBody->Off();
			return;
		}
	}
	for (int i = 6; i < 9; ++i)
	{
		auto FindIter = State()->m_MonsterMap.find(m_PlayerIndexArr[i].Index);
		auto EndIter = State()->m_MonsterMap.end();
		if (FindIter != EndIter)
		{
			m_PlayerLowHead->Off();
			m_PlayerLowBody->Off();
			m_PlayerHead->On();
			m_PlayerBody->On();
			m_PlayerHighHead->Off();
			m_PlayerHighBody->Off();
			return;
		}
	}

}

void Player::DirCheck()
{
	m_bKeyInput = true;
	m_bIsJump = false;

	if (GameInput::Down(L"MOVEL"))
	{
		m_bPressLeft = true;
		m_EState = PLAYERSTATE::MOVE;
		m_Dir = eDir::DIRL;
		m_bIsJump = true;
		return;
	}

	if (GameInput::Down(L"MOVER"))
	{
		m_bPressRight = true;
		m_EState = PLAYERSTATE::MOVE;
		m_Dir = eDir::DIRR;
		m_bIsJump = true;
		return;
	}

	if (GameInput::Down(L"MOVET"))
	{
		m_bPressUp = true;
		m_EState = PLAYERSTATE::MOVE;
		m_Dir = eDir::DIRT;
		m_bIsJump = true;
		return;
	}

	if (GameInput::Down(L"MOVEB"))
	{
		m_bPressDown = true;
		m_EState = PLAYERSTATE::MOVE;
		m_Dir = eDir::DIRB;
		m_bIsJump = true;
		return;
	}

	if (GameInput::Down(L"PlayerHealthSub"))
	{
		m_PlayerHealth -= 1;
	}

	m_bKeyInput = false;
}


void Player::DirChangeCheck()
{
	if (m_Dir != m_PrevDir)
	{
		m_PrevDir = m_Dir;
	}
}



//디버깅용 위치 체크
void Player::DebugRender()
{
	m_FPSUpdate -= GameTime::DeltaTime();
	if (0 >= m_FPSUpdate)
	{
		m_FPS = GameTime::FPS();
		m_FPSUpdate = 1.0f;
	}

	wchar_t Arr[256] = { 0, };
	swprintf_s(Arr, L"Pos : x : %f, y : %f, Edir : %d, FPS : %d", Pos().X, Pos().Y, m_Dir, m_FPS);
	TextOut(GameTexMgr::BackDC(), 0, 0, Arr, lstrlen(Arr));
	swprintf_s(Arr, L"CurPos : x : %f, y : %f, NextPos : x %f, y %f", m_CurPos.X, m_CurPos.Y, m_NextPos.X, m_NextPos.Y);
	TextOut(GameTexMgr::BackDC(), 0, 30, Arr, lstrlen(Arr));


	swprintf_s(Arr, L"camtimer : %f, Jumptimer : %f", GameState::m_CamTimer, m_JumpTimer);
	TextOut(GameTexMgr::BackDC(), 0, 60, Arr, lstrlen(Arr));


	swprintf_s(Arr, L"(%d,%d)", m_MapIndex.X, m_MapIndex.Y);
	TextOut(GameTexMgr::BackDC(), static_cast<int>(ActorCalcPos(Pos()).X), static_cast<int>(ActorCalcPos(Pos()).Y), Arr, lstrlen(Arr));

	swprintf_s(Arr, L"m_eState : %d, m_SoundIndex : %d, m_Gold %d", m_EState, m_SoundIndex, m_PlayerGold);
	TextOut(GameTexMgr::BackDC(), 0, 90, Arr, lstrlen(Arr));

	CVector tempMouse = GameWin::MainWindow()->MousePos();
	MouseX = static_cast<int>(std::round((tempMouse.X + State()->CamPos().X) / m_TileSize));
	MouseY = static_cast<int>(std::round((tempMouse.Y + State()->CamPos().Y) / m_TileSize));
	swprintf_s(Arr, L"MousePos %f, %f mouse index : %d, Y index : %d", tempMouse.X, tempMouse.Y, MouseX, MouseY);
	TextOut(GameTexMgr::BackDC(), 0, 120, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"CamPos X : %f Y : %f, MonsterMapSize %d, CanMove %d", State()->CamPos().X, State()->CamPos().Y, (int)State()->m_MonsterMap.size(), m_bCanMove);
	TextOut(GameTexMgr::BackDC(), 0, 150, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"XIndex X : %d YIndex : %dm, PlayerIndex X : %d, Y: %d", Xindex, Yindex, PlayerIndex.X, PlayerIndex.Y);
	TextOut(GameTexMgr::BackDC(), 0, 180, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"GlobalTime: %f", CoralHead::GlobalTime);
	TextOut(GameTexMgr::BackDC(), 0, 210, Arr, lstrlen(Arr));




	CVector DebugPos = ActorCalcPos(Pos());

	Rectangle(GameTexMgr::BackDC()
		, DebugPos.IX() - 3
		, DebugPos.IY() - 3
		, DebugPos.IX() + 3
		, DebugPos.IY() + 3
	);
}



void Player::InitAni()
{
	//삽
	m_Shovel = CreateRender<AniRender>((static_cast<int>(EActorTypeIndex::PlayerHigh)));
	m_Shovel->CreateAni(IDLE, L"shovel_basic.bmp", 0, 0, false, 0.2f);
	m_Shovel->SubSize(m_TileSize_Vector);
	m_Shovel->ChangeAni(IDLE);
	m_Shovel->Off();



#pragma region PlayerDefaultAni
	m_PlayerLowHead = CreateRender<AniRender>(static_cast<int>(EActorTypeIndex::PlayerLowHead));
	m_PlayerLowBody = CreateRender<AniRender>(static_cast<int>(EActorTypeIndex::PlayerLowBody));
	m_PlayerHead = CreateRender<AniRender>(static_cast<int>(EActorTypeIndex::PlayerHead));
	m_PlayerBody = CreateRender<AniRender>(static_cast<int>(EActorTypeIndex::PlayerBody));
	m_PlayerHighHead = CreateRender<AniRender>(static_cast<int>(EActorTypeIndex::PlayerHighHead));
	m_PlayerHighBody = CreateRender<AniRender>(static_cast<int>(EActorTypeIndex::PlayerHighBody));

	m_PlayerLowHead->CreateAni(IDLE, L"player1_heads.bmp", 0, 3, true, 0.1f);
	m_PlayerLowBody->CreateAni(IDLE, L"player1_armor_body_xmas.bmp", 0, 3, true, 0.1f);
	m_PlayerLowBody->CreateAni(OnArmor, L"player1_armor_body_xmas.bmp", 4, 7, true, 0.1f);
	m_PlayerLowHead->SubSize({ m_TileSize * 1.0f,m_TileSize * 1.0f });
	m_PlayerLowBody->SubSize({ m_TileSize* 1.0f,m_TileSize* 1.0f });
	m_PlayerLowHead->SubPos({ 0,0 });
	m_PlayerLowBody->SubPos({ 0,0 });
	m_PlayerLowHead->ChangeAni(IDLE);
	m_PlayerLowBody->ChangeAni(IDLE);
	m_PlayerLowHead->Off();
	m_PlayerLowBody->Off();


	m_PlayerHead->CreateAni(IDLE, L"player1_heads.bmp", 0, 3, true, 0.1f);
	m_PlayerBody->CreateAni(IDLE, L"player1_armor_body_xmas.bmp", 0, 3, true, 0.1f);
	m_PlayerBody->CreateAni(OnArmor, L"player1_armor_body_xmas.bmp", 4, 7, true, 0.1f);
	m_PlayerHead->SubSize({ m_TileSize * 1.0f,m_TileSize * 1.0f });
	m_PlayerBody->SubSize({ m_TileSize* 1.0f,m_TileSize* 1.0f });
	m_PlayerHead->SubPos({ 0,0 });
	m_PlayerBody->SubPos({ 0,0 });
	m_PlayerHead->ChangeAni(IDLE);
	m_PlayerBody->ChangeAni(IDLE);

	m_PlayerHighHead->CreateAni(IDLE, L"player1_heads.bmp", 0, 3, true, 0.1f);
	m_PlayerHighBody->CreateAni(IDLE, L"player1_armor_body_xmas.bmp", 0, 3, true, 0.1f);
	m_PlayerHighBody->CreateAni(OnArmor, L"player1_armor_body_xmas.bmp", 4, 7, true, 0.1f);
	m_PlayerHighHead->SubSize({ m_TileSize * 1.0f,m_TileSize * 1.0f });
	m_PlayerHighBody->SubSize({ m_TileSize* 1.0f,m_TileSize* 1.0f });
	m_PlayerHighHead->SubPos({ 0,0 });
	m_PlayerHighBody->SubPos({ 0,0 });
	m_PlayerHighHead->ChangeAni(IDLE);
	m_PlayerHighBody->ChangeAni(IDLE);
	m_PlayerHighHead->Off();
	m_PlayerHighBody->Off();
#pragma endregion


#pragma region PlayerSwipeAni
	PlayerSwipe[0] = CreateRender<AniRender>((int)EActorTypeIndex::Weapon);

	PlayerSwipe[0]->CreateAni(Dagger_Right, L"swipe_dagger_Right.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[0]->CreateAni(Dagger_Up, L"swipe_dagger_Up.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[0]->CreateAni(Dagger_Down, L"swipe_dagger_Down.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[0]->CreateAni(Dagger_Left, L"swipe_dagger_Left.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[0]->ChangeAni(Dagger_Left);
	PlayerSwipe[0]->SubSize(m_TileSize_Vector);

	PlayerSwipe[1] = CreateRender<AniRender>((int)EActorTypeIndex::Weapon);

	PlayerSwipe[1]->CreateAni(Rapier_Right, L"swipe_rapier_Right.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[1]->CreateAni(Rapier_Up, L"swipe_rapier_Up.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[1]->CreateAni(Rapier_Down, L"swipe_rapier_Down.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[1]->CreateAni(Rapier_Left, L"swipe_rapier_Left.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[1]->ChangeAni(Rapier_Left);
	PlayerSwipe[1]->SubSize(m_TileSize_Vector);

	PlayerSwipe[2] = CreateRender<AniRender>((int)EActorTypeIndex::Weapon);

	PlayerSwipe[2]->CreateAni(LongSword_Right, L"swipe_longsword_Right.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[2]->CreateAni(LongSword_Up, L"swipe_longsword_Up.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[2]->CreateAni(LongSword_Down, L"swipe_longsword_Down.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[2]->CreateAni(LongSword_Left, L"swipe_longsword_Left.bmp", 0, 3, false, 0.05f);
	PlayerSwipe[2]->ChangeAni(LongSword_Left);
	PlayerSwipe[2]->SubSize(m_TileSize_Vector);

	PlayerSwipe[3] = CreateRender<AniRender>((int)EActorTypeIndex::Weapon);

	PlayerSwipe[3]->CreateAni(BroadSword_Right, L"swipe_broadsword_Right.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[3]->CreateAni(BroadSword_Up, L"swipe_broadsword_Up.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[3]->CreateAni(BroadSword_Down, L"swipe_broadsword_Down.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[3]->CreateAni(BroadSword_Left, L"swipe_broadsword_Left.bmp", 0, 2, false, 0.05f);
	PlayerSwipe[3]->ChangeAni(BroadSword_Left);
	PlayerSwipe[3]->SubSize(m_TileSize_Vector);

	for (int i = 0; i < 4; ++i)
	{
		PlayerSwipe[i]->Off();
	}
#pragma endregion


	EnemySwipe = CreateRender<AniRender>((int)EActorTypeIndex::Player);

	EnemySwipe->CreateAni(MONSTER_ATTACK, L"swipe_enemy.bmp", 0, 4, false, 0.05f);
	EnemySwipe->ChangeAni(MONSTER_ATTACK);
	EnemySwipe->SubSize(m_TileSize_Vector);
	EnemySwipe->Off();


	m_Damaged = CreateRender<GameRender>((int)EActorTypeIndex::Player);
	m_Damaged->Image(L"Attack_red.bmp");
	m_Damaged->SubSize({ 960, 540 });
	m_Damaged->SubPos({ 0,0 });
	m_Damaged->Index(0);
	m_Damaged->ShadowImage(L"Attack_red_shadow.bmp");
	m_Damaged->ShadowIndex(0);
	m_Damaged->Off();
}

void Player::Damaged(int atkDmg)
{
	m_PlayerHealth -= atkDmg;
	EnemySwipe->On();
	EnemySwipe->ChangeAni(MONSTER_ATTACK);
	m_Damaged->On();
	GameSound::SoundPlayToOne(PlayerStateSound[1].c_str());

}


void Player::InitFSM()
{



}

void Player::DirChangeState(PLAYERSTATE _Change)
{
	m_EState = _Change;
}


void Player::OnItem(GameActor* _Item, InterActItem _ItemType)
{
	m_ExtTItem = _Item;
	m_ItemType = _ItemType;
	m_bOnItem = true;
}

bool Player::OnBuying(int _price)
{
	if (m_PlayerGold - _price > 0)
	{
		m_PlayerGold -= _price;
		return true;
	}

	return false;
}