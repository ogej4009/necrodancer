//#include "stdafx.h"
#include <GameTime.h>
#include <GameSound.h>
#include "BossRoom.h"
#include "Player.h"
#include "Background.h"
#include "MapEditor.h"
#include "Armor.h"
#include "Helmet.h"
#include "Weapon.h"

BossRoom::BossRoom()
{
}


BossRoom::~BossRoom()
{
}



void BossRoom::Loading()
{
	SetPlayer(new Player());

	Armor* InitArmor = new Armor({ 0 * GameActor::TileSize(), 0 * GameActor::TileSize() });

	GetPlayer()->CheckItem();
	/*Player()->OnItem(InitArmor, Player::InterActItem::BodyArmor);
	Player()->OnHead();
	Player()->ChangeHealth();

	Player()->ChangeWeaponType(3);*/
	Background* NewBack = new Background();
	m_BossRoom = new MapEditor(L"BOSSROOM.DATA", 1);

	InitUI();
}

void BossRoom::StateUpdate()
{

	//BossRoomSound->Stop();
	if (m_bMiniDie)
	{
		m_bMiniDie = false;
	}
}


void BossRoom::SoundOn()
{
	BossRoomSound = GameSound::SoundPlay(L"boss_1.mp3", -1);
}