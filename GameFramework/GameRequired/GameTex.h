#pragma once
#include <string>
#include <Windows.h>
#include "GameMath.h"


// �ε�� �̹��� �ϳ��� ��Ÿ����.
class GameFile;
class GameTex
{
	HDC m_Image;
	//CVector m_Size;

	HBITMAP m_hBitMap;
	HBITMAP m_hOldBitMap;
	BITMAPFILEHEADER m_BitFileHeader;
	BITMAPINFOHEADER m_BitInfo;
	std::wstring m_Name;

public:
	void Name(const wchar_t* _Name) { m_Name = _Name; }
	std::wstring Name() { return m_Name; }
	const std::wstring& CName() { return m_Name; }

public:
	CVector Size()
	{
		return { (float)m_BitInfo.biWidth, (float)m_BitInfo.biHeight };
	}


	HDC TEXDC() 
	{
		return m_Image;
	}

private:
	void CalTexSize(const std::wstring& _Path);

public:
	int GetTexPixel(int _X, int _Y);

public:
	bool Load(const GameFile& _File);
	bool Load(const std::wstring& _Path);

	//�ϴ� �� �ؽ��ĸ� ���� ���� ���۸� �뵵�� ���
	//main HDC��� ���� ���� ���
	bool Create(int _X, int _Y);


public:
	GameTex();
	~GameTex();
};

