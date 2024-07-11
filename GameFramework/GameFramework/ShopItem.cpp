//#include "stdafx.h"
#include "ShopItem.h"
#include "Helmet.h"
#include "Armor.h"
#include "Weapon.h"
#include "Text.h"

ShopItem::ShopItem(CVector _Pos)
{
	m_ActorTypeIndex = EActorTypeIndex::ShopItem;
	m_InitPos = _Pos;
	CVector tempPos = _Pos;
	CVector textPos = _Pos;
	Helmet* NewHelmet = new Helmet(_Pos);
	m_ItemIndex[0] = CalcIndex(_Pos);
	textPos.Y += m_TileSizeHalf;
	textPos.X += 14;
	m_Text[0] = new Text(textPos, { 10,10 }, L"30");

	tempPos.X += m_TileSize;
	textPos.X += m_TileSize;
	m_ItemIndex[1] = CalcIndex(tempPos);
	Armor* NewArmor = new Armor(tempPos);
	m_Text[1] = new Text(textPos, { 10,10 }, L"30");

	tempPos.X += m_TileSize;
	textPos.X += m_TileSize + 4;
	m_ItemIndex[2] = CalcIndex(tempPos);
	Weapon* NewWeapon = new Weapon(tempPos, Weapon::WeaponType::BroadSword);
	m_Text[2] = new Text(textPos, { 10,10 }, L"30");

	m_Item[0] = { NewHelmet, 30 };
	m_Item[1] = { NewArmor, 30 };
	m_Item[2] = { NewWeapon, 30 };

	NewHelmet->ShopItem();
	NewArmor->ShopItem();
	NewWeapon->ShopItem();

	Sell[0] = false;
	Sell[1] = false;
	Sell[2] = false;

}


ShopItem::~ShopItem()
{
}


void ShopItem::Update()
{
	if (State()->GetPlayer()->MapIndex() == m_ItemIndex[0] && !Sell[0])
	{
		if (State()->GetPlayer()->OnBuying(m_Item[0].price))
		{
			static_cast<Helmet*>(m_Item[0].ITEM)->ShopItem(false);
			Sell[0] = true;
			m_Text[0]->Death();
		}
	}
	else if (State()->GetPlayer()->MapIndex() == m_ItemIndex[1] && !Sell[1])
	{
		if (State()->GetPlayer()->OnBuying(m_Item[1].price))
		{
			static_cast<Armor*>(m_Item[1].ITEM)->ShopItem(false);
			Sell[1] = true;
			m_Text[1]->Death();
		}
	}
	else if (State()->GetPlayer()->MapIndex() == m_ItemIndex[2] && !Sell[2])
	{
		if (State()->GetPlayer()->OnBuying(m_Item[2].price))
		{
			static_cast<Weapon*>(m_Item[2].ITEM)->ShopItem(false);
			Sell[2] = true;
			m_Text[2]->Death();
		}
	}

}
