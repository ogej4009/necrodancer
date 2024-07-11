//#include "stdafx.h"
#include "UI_Text.h"


UI_Text::UI_Text(CVector _TargetPos, CVector _TextSize/* = (m_TileSize / 2.5f) - (m_TileSize / 10)*/, std::wstring _TextData/* = L"Null"*/)
{
	Pos({ 0,0 });
	m_ActorTypeIndex = EActorTypeIndex::UIText;


	int x = static_cast<int>(round(_TargetPos.X / m_TileSize));
	int y = static_cast<int>(round(_TargetPos.Y / m_TileSize));

	//그려지는 위치와 상관없이 처음에 찍은 위치정보
	m_TextPos.X = x;
	m_TextPos.Y = y;

	_TargetPos.X -= m_TileSizeHalf;
	_TargetPos.Y += m_TileSizeHalf / 2;
	CVector tempPos = _TargetPos;

	m_TextData = _TextData;

	int index = 0;

	for (size_t i = 0; i < _TextData.size(); ++i)
	{
		GameRender* NewRender = CreateRender<GameRender>(static_cast<int>(EActorTypeIndex::UIText));
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
			case ' ':
				index = 44;
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


UI_Text::~UI_Text()
{
}

void UI_Text::Update()
{
}


void UI_Text::ChangeSize(CVector _Size)
{
	m_StartIter = m_Text.begin();
	m_EndIter = m_Text.end();

	for (; m_StartIter != m_EndIter; ++m_StartIter)
	{
		(*m_StartIter)->SubSize(_Size);
	}
}

