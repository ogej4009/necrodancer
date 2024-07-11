#pragma once
#include <vector>
#include "GameFile.h" 


class GameDic : public GamePath
{
public:
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

	GameFile CreateGameFile(const wchar_t* _FileName);
	std::wstring CreateGameFilePath(const wchar_t* _FileName);

	bool Move(const wchar_t* _Name);
	void MoveParent();
	bool IsFile(const wchar_t* _FileName);
	std::vector<GameFile> DirToAllFile();
	std::vector<GameFile> DirToAllFile(const wchar_t* _Delimiter);

public:
	GameDic();
	~GameDic();
};

