//#include "stdafx.h"
#include "Armor.h"
#include "Player.h"

Armor::Armor(CVector _Pos) : m_Len(0)
{
	m_ActorTypeIndex = EActorTypeIndex::Armor;
	m_MapIndex = CalcIndex(_Pos);

	m_Armor = CreateRender<GameRender>((int)m_ActorTypeIndex);
	m_Armor->Image(L"armor_spiked_leather.bmp");
	m_Armor->SubSize(m_TileSize_Vector);
	m_Armor->SubPos(_Pos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}


Armor::~Armor()
{
}


void Armor::Update()
{
	if (!m_OnBuying)
	{
		if (State()->GetPlayer()->MapIndex() == m_MapIndex)
		{
			State()->GetPlayer()->OnItem(static_cast<GameActor*>(this), Player::InterActItem::BodyArmor);
			Death();
		}
	}
	

	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 9)
		{
			m_Armor->Index(1);
		}
		else
		{
			m_Armor->Index(0);
		}
	}
}