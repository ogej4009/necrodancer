//#include "stdafx.h"
#include "Gold_UI.h"
#include <GameWin.h>
#include "Text.h"
#include "GameState.h"
#include "Player.h"
#include "UI_Text.h"


Gold_UI::Gold_UI() : m_Position({ 0.0f, 0.0f }), m_PlayerGold(0)
{
	m_ActorTypeIndex = EActorTypeIndex::UI;
	m_Position.X = GameWin::MainWindow()->Size().X - (m_TileSize * 2.0f);
	m_Position.Y = m_TileSize - 10;

	InitRender(m_Position);
	InitText(m_Position);
}


Gold_UI::~Gold_UI()
{
}


void Gold_UI::Update()
{
	if (State()->GetPlayer()->PlayerGold() != m_PlayerGold)
	{
		m_PlayerGold = State()->GetPlayer()->PlayerGold();
		ChangeGold();
	}


}

void Gold_UI::ChangeGold()
{
	if (m_Text != nullptr)
	{
		m_TextCount->Death();
		m_TextCount = nullptr;
	}
	
	CVector _Pos = m_Position;

	_Pos.X += m_TileSize + m_TileSizeHalf - 1;
	_Pos.Y -= 8;

	m_TextCount = new UI_Text(_Pos, { m_TileSizeHalf - 5 ,m_TileSizeHalf }, std::to_wstring(m_PlayerGold));
	m_TextCount->ActorType(m_ActorTypeIndex);

}


void Gold_UI::InitRender(CVector _Pos)
{
	m_Gold = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
	m_Gold->SubPos(_Pos);
	m_Gold->SubSize({ m_TileSize ,m_TileSize });
	m_Gold->Image(L"hud_coins.bmp");
}

void Gold_UI::InitText(CVector _Pos)
{
	_Pos.X += m_TileSize + 5;
	_Pos.Y -= 5;

	m_Text = new UI_Text(_Pos, { 12.0f, 12.0f }, L"x");
	m_Text->ActorType(m_ActorTypeIndex);

	_Pos.X += m_TileSizeHalf - 4;
	_Pos.Y -= 3;

	m_TextCount = new UI_Text(_Pos, { m_TileSizeHalf - 5 ,m_TileSizeHalf }, L"0");
	m_TextCount->ActorType(m_ActorTypeIndex);

}