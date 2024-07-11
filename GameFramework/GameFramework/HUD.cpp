//#include "stdafx.h"
#include "HUD.h"
#include "GameRender.h"
#include "UI_Text.h"
#include "Player.h"



HUD::HUD() : m_Position({ 0.0f, 0.0f }), m_HudPadding(65.0f), m_PlayerWeaponType(0)
{
	m_Position.X = (m_TileSize * 1.0f);
	m_Position.Y = m_TileSizeHalf + 10.0f;
	m_ActorTypeIndex = EActorTypeIndex::UI;
	m_Size = m_TileSize * 1.5f;

	m_HudName[0] = L"Shovel";
	m_HudName[1] = L"Attack";
	m_HudName[2] = L"Head";
	m_HudName[3] = L"Body";

	m_HudName[4] = L"throw";
	m_HudName[5] = L"up +";
	m_HudName[6] = L"down";

	m_HudName[7] = L"Magic";
	m_HudName[8] = L"Num3";

	m_HudName[9] = L"Bomb";
	m_HudName[10] = L"Down";
	m_HudName[11] = L"Left";
	m_HudName[12] = L"x";
	m_HudName[13] = L"1";

	m_WeaponName[0] = L"weapon_dagger.bmp";
	m_WeaponName[1] = L"weapon_rapier.bmp";
	m_WeaponName[2] = L"weapon_longsword.bmp";
	m_WeaponName[3] = L"weapon_broadsword.bmp";

	InitRender(m_Position);
}


HUD::~HUD()
{
}


void HUD::Update()
{
	if (State()->GetPlayer()->CurWeaponType() != m_PlayerWeaponType)
	{
		ChagneWeaponImg(State()->GetPlayer()->CurWeaponType());
	}

	if (State()->GetPlayer()->CurHead())
	{
		TurnOnHud(Head);
	}

	if (State()->GetPlayer()->CurArmor())
	{
		TurnOnHud(Body);
	}

	
}



void HUD::ChagneWeaponImg(int _WeaponType)
{
	findIter = m_HudItem.find(Weapon);
	findIter2 = m_HudItem.find(Canthrow);
	findIter->second->Image(m_WeaponName[_WeaponType].c_str());
	findIter2->second->Image(m_WeaponName[_WeaponType].c_str());
	m_PlayerWeaponType = State()->GetPlayer()->CurWeaponType();
}


void HUD::TurnOnHud(int _HudNum)
{
	switch ((HudType)_HudNum)
	{
	case HUD::Shovel:
		break;
	case HUD::Weapon:
		break;
	case HUD::Head:
		findIter = m_HudItem.find(_HudNum);
		findIter->second->On();
		findIter = m_Hud.find(_HudNum);
		findIter->second->On();
		findTextIter = m_HudText.find(_HudNum);
		findTextIter->second->On();
	case HUD::Body:
		findIter = m_HudItem.find(_HudNum);
		findIter->second->On();
		findIter = m_Hud.find(_HudNum);
		findIter->second->On();
		findTextIter = m_HudText.find(_HudNum);
		findTextIter->second->On();
		break;
	case HUD::Canthrow:
		break;
	case HUD::Magic:
		break;
	case HUD::Bomb:
		break;
	default:
		break;
	}
}

void HUD::InitRender(CVector _Pos)
{

	m_SecondPosition = _Pos;
	m_SecondPosition.Y += m_Size * 1.5f;
	m_ThirdPosition = m_SecondPosition;
	m_ThirdPosition.Y += m_Size * 1.5f;

	//shovel
	GameRender* NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
	NewHud->SubPos(_Pos);
	NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
	NewHud->SubSize({ m_Size, m_Size * 1.2f });
	NewHud->Image(L"hud_slot_1.bmp");
	m_Hud.insert(std::map<int, GameRender*>::value_type(0, NewHud));

	UI_Text* newText = new UI_Text(_Pos, { 8.0f, 12.0f }, m_HudName[0]);
	newText->ChangeActorType(EActorTypeIndex::UIText);
	newText->Move({ 0.0f, -m_TileSize + 5.0f });
	m_HudText.insert(std::map<int, UI_Text*>::value_type(0, newText));

	GameRender* NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
	NewHudItem->SubPos(_Pos);
	NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 8.0f });
	NewHudItem->SubSize({ m_Size - 8.0f, m_Size - 8.0f });
	NewHudItem->Image(L"shovel_basic.bmp");
	m_HudItem.insert(std::map<int, GameRender*>::value_type(Shovel, NewHudItem));

	CVector tempPos = { 0,0 };


	//weapon
	{
		_Pos.X += m_HudPadding;
		NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHud->SubPos(_Pos);
		NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
		NewHud->SubSize({ m_Size, m_Size * 1.2f });
		NewHud->Image(L"hud_slot_1.bmp");
		m_Hud.insert(std::map<int, GameRender*>::value_type(1, NewHud));

		newText = new UI_Text(_Pos, { 8.0f, 12.0f }, m_HudName[1]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(1, newText));


		NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHudItem->SubPos(_Pos);
		NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 8.0f });
		NewHudItem->SubSize({ m_Size - 8.0f, m_Size - 8.0f });
		NewHudItem->Image(m_WeaponName[0].c_str());
		m_HudItem.insert(std::map<int, GameRender*>::value_type(Weapon, NewHudItem));

	}


	//head
	{
		_Pos.X += m_HudPadding;
		NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHud->SubPos(_Pos);
		NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
		NewHud->SubSize({ m_Size, m_Size * 1.2f });
		NewHud->Image(L"hud_slot_1.bmp");
		NewHud->Off();
		m_Hud.insert(std::map<int, GameRender*>::value_type(2, NewHud));

		tempPos = _Pos;
		tempPos.X += 8.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[2]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		newText->Off();
		m_HudText.insert(std::map<int, UI_Text*>::value_type(2, newText));


		NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHudItem->SubPos(_Pos);
		NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 8.0f });
		NewHudItem->SubSize({ m_Size - 8.0f, m_Size - 8.0f });
		NewHudItem->Image(L"head_blast_helm.bmp");
		NewHudItem->Off();
		m_HudItem.insert(std::map<int, GameRender*>::value_type(Head, NewHudItem));

	}


	//body
	{
		_Pos.X += m_HudPadding;
		NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHud->SubPos(_Pos);
		NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
		NewHud->SubSize({ m_Size, m_Size * 1.2f });
		NewHud->Image(L"hud_slot_1.bmp");
		NewHud->Off();
		m_Hud.insert(std::map<int, GameRender*>::value_type(3, NewHud));

		tempPos = _Pos;
		tempPos.X += 8.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[3]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		newText->Off();
		m_HudText.insert(std::map<int, UI_Text*>::value_type(3, newText));


		NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHudItem->SubPos(_Pos);
		NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 8.0f });
		NewHudItem->SubSize({ m_Size - 8.0f, m_Size - 8.0f });
		NewHudItem->Image(L"armor_spiked_leather.bmp");
		NewHudItem->Off();
		m_HudItem.insert(std::map<int, GameRender*>::value_type(Body, NewHudItem));

	}



	//throw
	{
		_Pos = m_SecondPosition;
		NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHud->SubPos(_Pos);
		NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
		NewHud->SubSize({ m_Size, m_Size * 1.2f });
		NewHud->Image(L"hud_slot_reload.bmp");
		m_Hud.insert(std::map<int, GameRender*>::value_type(4, NewHud));

		tempPos = _Pos;
		tempPos.X += 4.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[4]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(4, newText));


		NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHudItem->SubPos(_Pos);
		NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 2.0f });
		NewHudItem->SubSize({ m_Size - 8.0f, m_Size - 8.0f });
		NewHudItem->Image(m_WeaponName[0].c_str());
		m_HudItem.insert(std::map<int, GameRender*>::value_type(Canthrow, NewHudItem));

		tempPos = _Pos;
		tempPos.X += 8.0f;
		tempPos.Y += m_TileSize + 12.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[5]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(5, newText));

		tempPos = _Pos;
		tempPos.X += 4.0f;
		tempPos.Y += m_TileSize + 24.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[6]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(6, newText));

	}

	//magic
	{
		_Pos.X += m_HudPadding;
		NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHud->SubPos(_Pos);
		NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
		NewHud->SubSize({ m_Size, m_Size * 1.2f });
		NewHud->Image(L"hud_slot_reload.bmp");
		NewHud->Off();
		m_Hud.insert(std::map<int, GameRender*>::value_type(5, NewHud));


		tempPos = _Pos;
		tempPos.X += 4.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[7]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		newText->Off();
		m_HudText.insert(std::map<int, UI_Text*>::value_type(7, newText));

		tempPos = _Pos;
		tempPos.X += 8.0f;
		tempPos.Y += m_TileSize + 20.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[8]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		newText->Off();
		m_HudText.insert(std::map<int, UI_Text*>::value_type(8, newText));

		NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHudItem->SubPos(_Pos);
		NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 4.0f });
		NewHudItem->SubSize({ m_Size - 16.0f, m_Size - 16.0f });
		NewHudItem->Image(L"scroll_black.bmp");
		NewHudItem->Off();
		m_HudItem.insert(std::map<int, GameRender*>::value_type(Magic, NewHudItem));

	}


	//Bomb
	{
		_Pos = m_ThirdPosition;
		NewHud = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHud->SubPos(_Pos);
		NewHud->SubPosPure({ _Pos.X, _Pos.Y + 5.0f });
		NewHud->SubSize({ m_Size, m_Size * 1.2f });
		NewHud->Image(L"hud_slot_reload.bmp");
		m_Hud.insert(std::map<int, GameRender*>::value_type(5, NewHud));

		NewHudItem = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewHudItem->SubPos(_Pos);
		NewHudItem->SubPosPure({ _Pos.X, _Pos.Y + 2.0f });
		NewHudItem->SubSize({ m_Size - 18.0f, m_Size - 18.0f });
		NewHudItem->Image(L"bomb.bmp");
		m_HudItem.insert(std::map<int, GameRender*>::value_type(Bomb, NewHudItem));


		tempPos = _Pos;
		tempPos.X += 8.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[9]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(9, newText));

		tempPos = _Pos;
		tempPos.X += 8.0f;
		tempPos.Y += m_TileSize + 14.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[10]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(10, newText));

		tempPos = _Pos;
		tempPos.X += 8.0f;
		tempPos.Y += m_TileSize + 26.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[11]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(11, newText));


		tempPos = _Pos;
		tempPos.X += 34.0f;
		tempPos.Y += m_TileSize + 4.0f;
		newText = new UI_Text(tempPos, { 8.0f, 8.0f }, m_HudName[12]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(12, newText));

		tempPos = _Pos;
		tempPos.X += 42.0f;
		tempPos.Y += m_TileSize + 2.0f;
		newText = new UI_Text(tempPos, { 8.0f, 12.0f }, m_HudName[13]);
		newText->ChangeActorType(EActorTypeIndex::UIText);
		newText->Move({ 0.0f, -m_TileSize + 5.0f });
		m_HudText.insert(std::map<int, UI_Text*>::value_type(13, newText));

	}

}