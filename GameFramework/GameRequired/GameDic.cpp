#include "GameDic.h"
#include <atlstr.h>
#include <io.h>
#include "GameMacro.h"


GameDic::GameDic()
{
	wchar_t ArrSize[1024];

	if (0 == GetCurrentDirectoryW(1024, ArrSize))
	{
		ASSERTMSG;
	}

	PathSetting(ArrSize);

}


GameDic::~GameDic()
{
}


bool GameDic::IsFile(const wchar_t* _FileName)
{
	std::wstring FindPath = m_Path;
	FindPath += L"\\";
	FindPath += _FileName;

	return GamePath::IsExist(FindPath.c_str());
}

std::vector<GameFile> GameDic::DirToAllFile()
{
	return DirToAllFile(L"*.*");
}

std::vector<GameFile> GameDic::DirToAllFile(const wchar_t* _Delimiter)
{
	std::vector<GameFile> ReturnList;

	std::wstring FindPath = m_Path + L"\\" + _Delimiter;
	_wfinddata_t fd;

	std::wstring Check;

	intptr_t handle = _wfindfirst(FindPath.c_str(), &fd);
	do
	{
		Check = L".";
		if (fd.name == Check)
		{
			continue;
		}
		Check = L"..";
		if (fd.name == Check)
		{
			continue;
		}

		GameFile NewFile;
		NewFile.PathSetting(m_Path + L"\\" + fd.name);
		ReturnList.push_back(NewFile);
	} while (0 == _wfindnext(handle, &fd));

	_findclose(handle); // <io.h>

	return ReturnList;
}

void GameDic::PathSetting(const char* _Path)
{
	PathSetting(CA2W(_Path).m_psz);
}

bool GameDic::Move(const wchar_t* _Name)
{
	std::wstring MovePath = m_Path;
	MovePath += L"\\";
	MovePath += _Name;

	if (false == IsDir(MovePath.c_str()))
	{
		ASSERTMSG;
		return false;
	}

	m_Path = MovePath;

	return true;
}

void GameDic::MoveParent()
{
	size_t LastFolderIndex = m_Path.find_last_of(L"\\");
	m_Path.replace(LastFolderIndex, m_Path.size() - 1, L"");
}

void GameDic::PathSetting(const wchar_t* _Path)
{
	if (0 != (FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(_Path)))
	{
		m_Path = _Path; //디렉토리 경로
		return;
	}

	m_Path = _Path;
	MoveParent(); //파일경로를 받은 경우

}

GameFile GameDic::CreateGameFile(const wchar_t* _FileName)
{
	std::wstring CreatePath = CreateGameFilePath(_FileName);
	GameFile NewFile;
	NewFile.Open(CreatePath.c_str(), L"wb");
	NewFile.Close();
	return GameFile(NewFile);
}


std::wstring GameDic::CreateGameFilePath(const wchar_t* _FileName)
{
	return m_Path + L"\\" + _FileName;
}
