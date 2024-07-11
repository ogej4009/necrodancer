//#include "stdafx.h"
#include "ShopKeeper.h"
#include "Player.h"
#include "GameSound.h"
#include <GameTime.h>


unsigned int ShopKeeper::m_KeeperTime = 0;

ShopKeeper::ShopKeeper(CVector _TargetPos) : m_bClosePlayer(false), m_bPlayMusic(false), m_Start(false)
{
	m_ActorTypeIndex = EActorTypeIndex::ShopKeeper;
	InitAni(_TargetPos);

	m_MapIndex = m_Keeper->SubPosIndeX();
	
}


ShopKeeper::~ShopKeeper()
{
}



void ShopKeeper::InitAni(CVector _TargetPos)
{
	m_Keeper = CreateRender<AniRender>((int)m_ActorTypeIndex);
	m_Keeper->CreateAni(IDLE, L"shopkeeper.bmp", 0, 7, true, 0.2f);
	m_Keeper->CreateAni(ShadowIDLE, L"shopkeeper.bmp", 8, 14, true, 0.2f);
	m_Keeper->SubPos(_TargetPos);
	CVector subPosPure = _TargetPos;
	subPosPure.Y -= m_TileSizeHalf;
	m_Keeper->SubPosPure(subPosPure);
	subPosPure = m_TileSize_Vector * 1.5f;
	subPosPure.X += 10;
	m_Keeper->SubSize(subPosPure);
	m_Keeper->ChangeAni(IDLE);

}


void ShopKeeper::CheckPlayer()
{
	if ((State()->GetPlayer()->PlayerIndex - m_MapIndex).IndexLen() < 12)
	{
		m_bClosePlayer = true;
	}
	else
	{
		m_bClosePlayer = false;
	}
}


void ShopKeeper::Update()
{
	if (State()->m_StateChanged)
	{
		if (!m_Start)
		{
			m_KeeperSong = GameSound::SoundPlay(L"zone1_1_shopkeeper.mp3", -1);
			m_KeeperSong->SoundVolume(0);
			m_Start = true;
		}
	

		if (m_bClosePlayer && !m_bPlayMusic)
		{
			m_KeeperSong->SoundVolume(1.0f);
			m_bPlayMusic = true;
		}
		else
		{

			if (!m_bClosePlayer && m_bPlayMusic)
			{
				unsigned int tempNum = (int)((10 - m_Len) / 10);
				if (m_Len > 8)
				{
					tempNum = 0;
				}
				m_KeeperSong->SoundVolume((float)tempNum);
			}

		}
	}


	CheckPlayer();
	m_Time += GameTime::DeltaTime();

	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 7)
		{
			m_Keeper->ChangeAni(ShadowIDLE);
		}
		else
		{
			m_Keeper->ChangeAni(IDLE);
			m_firstContact = true;
		}
	}

	if (m_firstContact)
	{
		m_Keeper->On();
	}
	else
	{
		m_Keeper->Off();
	}



	

}