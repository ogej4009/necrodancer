//#include "stdafx.h"
#include "Torch.h"
#include <GameTime.h>

Torch::Torch(CVector _TargetPos)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::Walltorch;
	MapIndex(_TargetPos);

	State()->m_InterObjMap.insert(std::map<__int64, GameActor*>::value_type(
		m_MapIndex.Index, static_cast<GameActor*>(this)));

	InitAni(_TargetPos);
}

Torch::~Torch()
{
}


void Torch::Update()
{
	if (State()->m_WallActorMap.find(m_MapIndex.Index) == State()->m_WallActorMap.end())
	{
		wchar_t buff[100];
		swprintf_s(buff, L"index : %d, index : %d\n", m_MapIndex.X, m_MapIndex.Y);
		OutputDebugStringW(buff);
		Death();
	}

	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 7)
		{
			m_Torch->Off();
		}
		else
		{
			m_Torch->On();
		}
	}
}

void Torch::InitAni(CVector _TargetPos)
{
	m_Torch = CreateRender<AniRender>(static_cast<int>(m_ActorTypeIndex));
	m_Torch->CreateAni(Torchi, L"wall_torch.bmp", 0, 3, true, 0.07f);
	m_Torch->SubPos(_TargetPos);
	_TargetPos.Y -= m_TileSizeHalf + m_TileSizeHalf / 2;
	m_Torch->SubPosPure(_TargetPos);
	m_Torch->SubSize({ m_TileSize / 2.0f , m_TileSize });

	m_Torch->ChangeAni(Torchi);
}