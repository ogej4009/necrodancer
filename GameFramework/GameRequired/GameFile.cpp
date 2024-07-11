#include "GameFile.h"
#include <atlstr.h>


GameFile::GameFile(const wchar_t* _Path, const wchar_t* _Mode) : m_pFile(nullptr)
{
	Open(_Path, _Mode);
}

GameFile::GameFile(const GameFile& _Other) : m_pFile(nullptr)
{
	Copy(_Other);
}

GameFile::GameFile() : m_pFile(nullptr)
{
}


GameFile::~GameFile()
{
	Close();
}

void GameFile::PathSetting(const char* _Path)
{
	PathSetting(CA2W(_Path).m_psz);
}

void GameFile::PathSetting(const std::wstring& _Path)
{
	PathSetting(_Path.c_str());
}

void GameFile::PathSetting(const wchar_t* _Path)
{
	m_Path = _Path;
	m_FileName = m_Path;
	size_t LastFolderIndex = m_FileName.find_last_of(L"\\");
	m_FileName = m_FileName.replace(0, LastFolderIndex + 1, L"");

	m_Ext = m_FileName;
	LastFolderIndex = m_Ext.find_last_of(L".");
	m_Ext = m_Ext.replace(0, LastFolderIndex + 1, L"");
}

bool GameFile::Open()
{
	return Open(m_Path.c_str(), m_OpenMode.c_str());
}

bool GameFile::Open(const wchar_t* _Path, const wchar_t* _Mode)
{
	PathSetting(_Path);
	m_OpenMode = _Mode;
	_wfopen_s(&m_pFile, m_Path.c_str(), m_OpenMode.c_str());

	if (nullptr == m_pFile)
	{
		assert(false);
		return false;
	}

	return true;
}

void GameFile::Close()
{
	if (nullptr != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

void GameFile::Copy(const GameFile& _Other)
{
	PathSetting(_Other.m_Path);
	m_pFile = _Other.m_pFile;
}

