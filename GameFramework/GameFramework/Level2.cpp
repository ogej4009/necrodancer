//#include "stdafx.h"
#include "Level2.h"
#include "Player.h"
#include "Background.h"
#include "MapEditor.h"
#include <GameSound.h>
#include "Armor.h"
#include "Helmet.h"
#include "Weapon.h"



Level2::Level2() : Check(false)
{
	m_NextLevel = { 17,-6 };
}


Level2::~Level2()
{
}

void Level2::Loading()
{
	///
	SetPlayer(new Player());
	
	Armor* InitArmor = new Armor({0* GameActor::TileSize(), 0 * GameActor::TileSize() });
	
	
	/*Player()->OnItem(InitArmor, Player::InterActItem::BodyArmor);
	Player()->OnHead();
	Player()->ChangeHealth();

	Player()->ChangeWeaponType(3);*/
	Background* NewBack = new Background();
	m_Level2 = new MapEditor(L"Level2.DATA");
	InitUI();
}

void Level2::StateUpdate()
{

	if (GetPlayer()->OnStair() && GetPlayer()->MapIndex() == m_NextLevel)
	{
		GetPlayer()->ChangeGlobal();
		ChangeState(L"BOSSROOM");
		L2Sound->Stop();
		TurnOffUI();
	}
}


void Level2::SoundOn()
{
	L2Sound = GameSound::SoundPlay(L"zone1_2.mp3", -1);
}