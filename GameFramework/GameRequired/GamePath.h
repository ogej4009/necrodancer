#pragma once
#include <string>
#include <assert.h>
#include <Windows.h>


class GamePath
{
public:
	static bool IsExist(const wchar_t* _Path)
	{
		if (0 != _waccess_s(_Path, 0))
		{
			return false;
		}
		return true;
	}

	static bool IsDir(const wchar_t* _Path)
	{
		int Result = GetFileAttributesW(_Path);

		if (-1 == Result)
		{
			return false;
		}

		if (0 == (FILE_ATTRIBUTE_DIRECTORY & Result))
		{
			return false;
		}

		return true;
	}


protected:
	std::wstring m_Path;

public:
	const std::wstring& CFullPath() const
	{
		return m_Path;
	}

public:
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

public:
	GamePath();
	~GamePath();
};

