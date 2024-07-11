#pragma once
#include "GameMath.h"
#include <Windows.h>
#include <map>
#include <string>


class GameFile;
class GameTex;
class GameImage;
class GameTexMgr
{
	//싱글톤
public:
	static GameTexMgr& Inst() {
		static GameTexMgr TextInst;
		return TextInst;
	}

private:
	static CVector m_BackDCSize;
	//main HDC대신 그려질 HDC
	static HDC m_BackDC;

public:
	static const CVector& BackDCSize()
	{
		return m_BackDCSize;
	}

	static const HDC& BackDC()
	{
		return m_BackDC;
	}
private:
	GameTex* m_BackBufferTex;
	//원본에 가까운 이미지
	std::map<std::wstring, GameTex* > m_AllTex;
	//짤라서 쓰고싶어하는 이미지
	std::map<std::wstring, GameImage* > m_AllImage;

public:
	void InitTexMgr();
	
public:
	GameTex* LoadTex(const GameFile& _File);
	GameTex* FindTex(const std::wstring& _Key);

	//자르고싶은 텍스쳐와 X,Y 횟수,사각형으로 몇조각 낼거니?
	GameImage* CreateImage(const wchar_t* _TexName, unsigned int _X, unsigned int _Y);
	//자른 위치정보를 가지고 있는 Image 클래스를 찾아주는 함수
	GameImage* FindImg(const std::wstring& _Key);

private:
	GameTexMgr();
	~GameTexMgr();
};

