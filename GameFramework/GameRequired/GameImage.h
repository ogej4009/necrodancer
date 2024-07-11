#pragma once
#include <vector>
#include "GameTex.h"

//�ϳ��� ū �ؽ��ĸ� �߶���� �̹����鿡 ���� 
//��ġ�� ũ�������� GameRect ���·� ������ �ִ� Ŭ����
//���� �̹����� Tex�� �ڸ��� ���·� �������� �ȴ�.

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

