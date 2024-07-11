//#include "stdafx.h"
#include "Diamond_UI.h"
#include <GameWin.h>
#include "GameRender.h"
#include "UI_Text.h"


Diamond_UI::Diamond_UI() : m_Position({ 0.0f, 0.0f })
{
	m_Position.X = GameWin::MainWindow()->Size().X - (m_TileSize * 2.0f);
	m_Position.Y = m_TileSize * 2 - 10;
	m_ActorTypeIndex = EActorTypeIndex::UI;

	InitRender(m_Position);
	InitText(m_Position);
}


Diamond_UI::~Diamond_UI()
{
}


void Diamond_UI::Update()
{

}

void Diamond_UI::InitRender(CVector _Pos)
{
	m_Diamond = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
	m_Diamond->SubPos(_Pos);
	m_Diamond->SubSize({ m_TileSize ,m_TileSize });
	m_Diamond->Image(L"diamond.bmp");
}

void Diamond_UI::InitText(CVector _Pos)
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