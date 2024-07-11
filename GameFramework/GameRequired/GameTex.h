#pragma once
#include <string>
#include <Windows.h>
#include "GameMath.h"


// 로드된 이미지 하나를 나타낸다.
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

	//일단 빈 텍스쳐를 만들어서 더블 버퍼링 용도로 사용
	//main HDC대신 만들어서 붙일 대상
	bool Create(int _X, int _Y);


public:
	GameTex();
	~GameTex();
};

