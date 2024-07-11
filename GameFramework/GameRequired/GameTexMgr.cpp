#include "GameTexMgr.h"
#include "GameFile.h"
#include "GameTex.h"
#include "GameMacro.h"
#include "GameWin.h"
#include "GameImage.h"


CVector GameTexMgr::m_BackDCSize;
HDC GameTexMgr::m_BackDC = nullptr;

GameTexMgr::GameTexMgr()
{
}


GameTexMgr::~GameTexMgr()
{
	std::map<std::wstring, GameTex*>::iterator TexStart = m_AllTex.begin();
	std::map<std::wstring, GameTex*>::iterator TexEnd = m_AllTex.end();

	for (; TexStart != TexEnd; ++TexStart)
	{
		if (nullptr != TexStart->second)
		{
			delete TexStart->second;
			TexStart->second = nullptr;
		}
	}
	m_AllTex.clear();

	if (nullptr != m_BackBufferTex)
	{
		delete m_BackBufferTex;
		m_BackBufferTex = nullptr;
	}

	auto imageStart = m_AllImage.begin();
	auto imageEnd = m_AllImage.end();

	for (; imageStart != imageEnd; ++imageStart)
	{
		if (nullptr != imageStart->second)
		{
			delete imageStart->second;
			imageStart->second = nullptr;
		}
	}
	m_AllImage.clear();


}

GameTex* GameTexMgr::LoadTex(const GameFile& _File)
{
	GameTex* NewTex = new GameTex();

	if (false == NewTex->Load(_File.CFullPath()))
	{
		ASSERTMSG;
	}
	
	NewTex->Name(_File.CFileName().c_str());
	m_AllTex.insert(std::map<std::wstring, GameTex*>::value_type(_File.CFileName(), NewTex));


	return NewTex;
}


GameTex* GameTexMgr::FindTex(const std::wstring& _Key)
{
	std::map<std::wstring, GameTex*>::iterator FindTexIter = m_AllTex.find(_Key);

	if (FindTexIter == m_AllTex.end())
	{
		return nullptr;
	}

	if (nullptr == FindTexIter->second)
	{
		ASSERTMSG;
	}

	return FindTexIter->second;
}


void GameTexMgr::InitTexMgr()
{
	m_BackBufferTex = new GameTex();

	if (false == m_BackBufferTex->Create(GameWin::MainWindow()->Size().IX(), GameWin::MainWindow()->Size().IY()))
	{
		ASSERTMSG;
	}
	//m_Image (HDC)¹ÝÈ¯
	m_BackDC = m_BackBufferTex->TEXDC();
	m_BackDCSize = GameWin::MainWindow()->Size();
	
}

GameImage* GameTexMgr::CreateImage(const wchar_t* _TexName, unsigned int _X, unsigned int _Y)
{
	GameTex* pFindTex = FindTex(_TexName);

	if (nullptr == pFindTex)
	{
		ASSERTMSG;
		return nullptr;
	}

	if (nullptr != FindImg(pFindTex->Name()))
	{
		ASSERTMSG;
		return nullptr;
	}

	GameImage* NewImage = new GameImage;
	NewImage->Cut(pFindTex, _X, _Y);

	m_AllImage.insert(std::map<std::wstring, GameImage*>::value_type(pFindTex->Name(), NewImage));
	return NewImage;
}

GameImage* GameTexMgr::FindImg(const std::wstring& _Key)
{
	auto FindImage = m_AllImage.find(_Key);

	if (FindImage == m_AllImage.end())
	{
		return nullptr;
	}

	return FindImage->second;

}