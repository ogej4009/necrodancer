//#include "stdafx.h"
#include "Text.h"
#include "Player.h"


Text::Text(CVector _TargetPos, CVector _TextSize/* = (m_TileSize / 2.5f) - (m_TileSize / 10)*/, std::wstring _TextData/* = L"Null"*/) : m_Len(0)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::Text;
	MapIndex(_TargetPos);

	int x = static_cast<int>(std::round(_TargetPos.X / m_TileSize));
	int y = static_cast<int>(std::round(_TargetPos.Y / m_TileSize));

	//그려지는 위치와 상관없이 처음에 찍은 위치정보
	m_TextPos.X = x;
	m_TextPos.Y = y;

	_TargetPos.X -= m_TileSizeHalf;
	_TargetPos.Y += m_TileSizeHalf / 2;
	CVector tempPos = _TargetPos;

	m_TextData = _TextData;

	int index = 0;

	for (int i = 0; i < static_cast<int>(_TextData.size()); ++i)
	{
		GameRender* NewRender = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::Text));
		NewRender->SubPos(tempPos);
		NewRender->SubSize({ _TextSize.X, _TextSize.Y });
		NewRender->Image(L"necrosans_12_black.bmp");
		tempPos.X += _TextSize.X;

		if (L'0' <= _TextData[i] && _TextData[i] <= L'9')
		{
			index = static_cast<int>(_TextData[i] - L'0');
			NewRender->Index(index);
		}
		else if (L'A' <= _TextData[i] && _TextData[i] <= L'Z')
		{
			index = static_cast<int>(_TextData[i] - L'A') + 10;
			NewRender->Index(index);
		}
		else if (L'a' <= _TextData[i] && _TextData[i] <= L'z')
		{
			index = static_cast<int>(_TextData[i] - L'a') + 10;

			NewRender->Index(index);
		}
		else
		{
			switch (_TextData[i])
			{
			case '+':
				index = 39;
				NewRender->Index(index);
				break;
			case '-':
				index = 38;
				NewRender->Index(index);
				break;
			default:
				ASSERTMSG;
				break;
			}
			continue;
		}

		m_Text.push_back(NewRender);
	}
}


Text::Text(CVector _TargetPos, CVector _TextSize/* = (m_TileSize / 2.5f) - (m_TileSize / 10)*/, std::wstring _TextData/* = L"Null"*/, EActorTypeIndex _Type) : m_Len(0)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = _Type;
	MapIndex(_TargetPos);

	int x = static_cast<int>(std::round(_TargetPos.X / m_TileSize));
	int y = static_cast<int>(std::round(_TargetPos.Y / m_TileSize));

	//그려지는 위치와 상관없이 처음에 찍은 위치정보
	m_TextPos.X = x;
	m_TextPos.Y = y;

	_TargetPos.X -= m_TileSizeHalf;
	_TargetPos.Y += m_TileSizeHalf / 2;
	CVector tempPos = _TargetPos;

	m_TextData = _TextData;

	int index = 0;

	for (int i = 0; i < static_cast<int>(_TextData.size()); ++i)
	{
		GameRender* NewRender = CreateRender<GameRender>(static_cast<int>(m_ActorTypeIndex));
		NewRender->SubPos(tempPos);
		NewRender->SubSize({ _TextSize.X, _TextSize.Y });
		NewRender->Image(L"necrosans_12_black.bmp");
		tempPos.X += _TextSize.X;

		if (L'0' <= _TextData[i] && _TextData[i] <= L'9')
		{
			index = static_cast<int>(_TextData[i] - L'0');
			NewRender->Index(index);
		}
		else if (L'A' <= _TextData[i] && _TextData[i] <= L'Z')
		{
			index = static_cast<int>(_TextData[i] - L'A') + 10;
			NewRender->Index(index);
		}
		else if (L'a' <= _TextData[i] && _TextData[i] <= L'z')
		{
			index = static_cast<int>(_TextData[i] - L'a') + 10;

			NewRender->Index(index);
		}
		else
		{
			switch (_TextData[i])
			{
			case '+':
				index = 39;
				NewRender->Index(index);
				break;
			case '-':
				index = 38;
				NewRender->Index(index);
				break;
			default:
				ASSERTMSG;
				break;
			}
			continue;
		}

		m_Text.push_back(NewRender);
	}
}



Text::~Text()
{
}

void Text::Update()
{
	if (m_Len != State()->m_TileRender->Len(m_MapIndex))
	{
		m_Len = State()->m_TileRender->Len(m_MapIndex);

		if (m_Len > 9)
		{
			for (int i = 0; i < m_Text.size(); ++i)
			{
				m_Text[i]->Off();
			}
		
		}
		else
		{
			for (int i = 0; i < m_Text.size(); ++i)
			{
				m_Text[i]->On();
			}
		}
	}



}


void Text::ChangeSize(CVector _Size)
{
	m_StartIter = m_Text.begin();
	m_EndIter = m_Text.end();

	for (; m_StartIter != m_EndIter; ++m_StartIter)
	{
		(*m_StartIter)->SubSize(_Size);
	}
}
