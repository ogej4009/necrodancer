#include "GameImage.h"
#include "GameMacro.h"


GameImage::GameImage()
{
}

GameImage::~GameImage()
{
}

void GameImage::Cut(GameTex* _pTex, int _X, int _Y)
{
	if (nullptr == _pTex)
	{
		ASSERTMSG;
	}

	if (1 > _X)
	{
		ASSERTMSG;
	}

	if (1 > _Y)
	{
		ASSERTMSG;
	}

	m_pTex = _pTex;
	m_DC = m_pTex->TEXDC();

	CVector CutSize = m_pTex->Size();

	CutSize.X = CutSize.X / _X;
	CutSize.Y = CutSize.Y / _Y;

	CVector StartPos = CVector::ZERO;

	for (int Y = 0; Y < _Y; ++Y)
	{
		for (int X = 0; X < _X; ++X)
		{
			GameRect NewRect;

			NewRect.Pos = { StartPos.X, StartPos.Y };
			NewRect.Size = CutSize;

			//�߶�޶�� �ϴ� �簢���� ��ġ������ ũ�������� ������ �ִ´�.
			m_AllCutData.push_back(NewRect);

			StartPos.X += CutSize.X;
		}

		StartPos.X = 0.0f;
		StartPos.Y += CutSize.Y;
	}

}