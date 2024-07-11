#pragma once
#include <vector>
#include "GameTex.h"

//하나의 큰 텍스쳐를 잘라놓은 이미지들에 대한 
//위치와 크기정보를 GameRect 형태로 가지고 있는 클래스
//실제 이미지는 Tex를 자르는 형태로 가져오게 된다.

class GameTexMgr;
class GameImage
{
private:
	friend GameTexMgr;

private:
	GameTex* m_pTex;
	HDC m_DC;
	std::vector<GameRect> m_AllCutData;

public:
	size_t CutCount() 
	{
		return m_AllCutData.size();
	}

	GameRect GetRect(int _Index)
	{
		return m_AllCutData[_Index];
	}

	HDC HDC()
	{
		return m_DC;
	}

private:
	void Cut(GameTex* _pTex, int _X, int _Y);

public:
	GameImage();
	~GameImage();
};

