//#include "stdafx.h"
#include "Background.h"


Background::Background()
{
	m_ActorTypeIndex = EActorTypeIndex::Background;
	m_Background = CreateRender<GameRender>((int)EActorTypeIndex::Background);
	m_Background->Image(L"TEMP_overlay_black.bmp");
	m_Background->SubPos({ 480,270 });
	m_Background->SubSize({ 960,540 });
}


Background::~Background()
{
}
