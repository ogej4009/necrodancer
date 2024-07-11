//#include "stdafx.h"
#include "Chest.h"
#include "Weapon.h"



Chest::Chest(CVector _TargetPos, int _WeaponType) :m_WeaponType(0)
{
	m_ActorTypeIndex = EActorTypeIndex::Chest;

	m_Chest = CreateRender<GameRender>((int)m_ActorTypeIndex);
	m_Chest->Image(L"addchest_black.bmp");
	m_Chest->Index(0);
	m_Chest->SubPos(_TargetPos);
	m_Chest->SubSize(m_TileSize_Vector);
	m_MapIndex = m_Chest->SubPosIndeX();
	m_WeaponType = _WeaponType;

	State()->m_MonsterMap.insert(std::map<__int64, GameActor*>::value_type(m_MapIndex.Index, this));
}


Chest::~Chest()
{
	State()->m_MonsterMap.erase(m_MapIndex.Index);
}


void Chest::Update()
{

	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 9)
		{
			m_Chest->Index(1);
		}
		else
		{
			m_Chest->Index(0);
		}
	}
}

void Chest::OpenChest()
{
	Weapon* NewWeapon = new Weapon({ m_MapIndex.X * m_TileSize, m_MapIndex.Y * m_TileSize }, (Weapon::WeaponType)m_WeaponType);
	Death();
}