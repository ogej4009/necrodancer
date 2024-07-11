//#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"



Weapon::Weapon(CVector _Pos, WeaponType _WeaponType) : m_bCanChange(true), m_Len(0), m_OnBuying(false)
{
	m_ActorTypeIndex = EActorTypeIndex::Weapon;
	m_WeaponType = _WeaponType;
	m_MapIndex = CalcIndex(_Pos);
	InitRender(_Pos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}


Weapon::~Weapon()
{
}

void Weapon::Update()
{

	if (!m_OnBuying)
	{
		//플레이어가 내 위로 왔다면 플레이어의 WeaponType으로 내 이미지를 바꾸자.
		if (State()->GetPlayer()->MapIndex() == m_MapIndex)
		{
			if (m_bCanChange)
			{
				State()->GetPlayer()->OnItem(static_cast<GameActor*>(this), Player::InterActItem::Weapon);
			}
			m_bCanChange = false;
		}
		else
		{
			m_bCanChange = true;
		}
		
	}

	


	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 9)
		{
			m_Weapon->Index(1);
		}
		else
		{
			m_Weapon->Index(0);
		}
	}
}


void Weapon::InitRender(CVector _Pos)
{

	m_Weapon = CreateRender<GameRender>((int)m_ActorTypeIndex);
	m_Weapon->SubPos(_Pos);
	m_Weapon->SubSize({ m_TileSize_Vector });
	SwtichImg();

	m_Weapon->Index(0);

}


int Weapon::CurType(int _WeaponType)
{
	Weapon::WeaponType RetType = m_WeaponType;
	m_WeaponType = static_cast<Weapon::WeaponType>(_WeaponType);
	SwtichImg();
	return (int)RetType;
}

void Weapon::SwtichImg()
{
	switch (m_WeaponType)
	{
	case Weapon::WeaponType::Dagger:
		m_Weapon->Image(L"weapon_dagger.bmp");
		break;
	case Weapon::WeaponType::Rapier:
		m_Weapon->Image(L"weapon_rapier.bmp");
		break;
	case Weapon::WeaponType::LongSword:
		m_Weapon->Image(L"weapon_longsword.bmp");
		break;
	case Weapon::WeaponType::BroadSword:
		m_Weapon->Image(L"weapon_broadsword.bmp");
		break;
	default:
		break;
	}
}