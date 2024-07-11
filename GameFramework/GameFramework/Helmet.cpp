//#include "stdafx.h"
#include "Helmet.h"
#include "Player.h"



Helmet::Helmet(CVector _Pos)
{
	m_ActorTypeIndex = EActorTypeIndex::Helmet;
	m_MapIndex = CalcIndex(_Pos);

	m_Helmet = CreateRender<GameRender>((int)m_ActorTypeIndex);
	m_Helmet->Image(L"head_blast_helm.bmp");
	m_Helmet->SubSize(m_TileSize_Vector);
	m_Helmet->SubPos(_Pos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));
}


Helmet::~Helmet()
{
}



void Helmet::Update()
{
	if (!m_OnBuying)
	{
		if (State()->GetPlayer()->MapIndex() == m_MapIndex)
		{
			State()->GetPlayer()->OnItem(static_cast<GameActor*>(this), Player::InterActItem::Helmet);
			Death();
		}
	}

	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 9)
		{
			m_Helmet->Index(1);
		}
		else
		{
			m_Helmet->Index(0);
		}
	}
}