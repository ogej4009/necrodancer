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
	//�̱���
public:
	static GameTexMgr& Inst() {
		static GameTexMgr TextInst;
		return TextInst;
	}

private:
	static CVector m_BackDCSize;
	//main HDC��� �׷��� HDC
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
	//������ ����� �̹���
	std::map<std::wstring, GameTex* > m_AllTex;
	//©�� ����;��ϴ� �̹���
	std::map<std::wstring, GameImage* > m_AllImage;

public:
	void InitTexMgr();
	
public:
	GameTex* LoadTex(const GameFile& _File);
	GameTex* FindTex(const std::wstring& _Key);

	//�ڸ������ �ؽ��Ŀ� X,Y Ƚ��,�簢������ ������ ���Ŵ�?
	GameImage* CreateImage(const wchar_t* _TexName, unsigned int _X, unsigned int _Y);
	//�ڸ� ��ġ������ ������ �ִ� Image Ŭ������ ã���ִ� �Լ�
	GameImage* FindImg(const std::wstring& _Key);

private:
	GameTexMgr();
	~GameTexMgr();
};

