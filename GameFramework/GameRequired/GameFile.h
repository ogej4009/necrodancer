#pragma once
#include <iostream>
#include "GamePath.h"


class GameFile : public GamePath
{
private:
	std::wstring m_FileName;
	std::wstring m_Ext;
	std::wstring m_OpenMode;

	FILE* m_pFile;

public:
	template<typename T>
	void Write(const T& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}
		if (false == IsWrite())
		{
			assert(false);
		}
		fwrite(&_Data, sizeof(T), 1, m_pFile);
	}

	//template Æ¯¼öÈ­
	template<>
	void Write(const std::wstring& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsWrite())
		{
			assert(false);
		}

		int Size = (int)_Data.size();
		fwrite(&Size, sizeof(int), 1, m_pFile);
		fwrite(_Data.c_str(), sizeof(wchar_t) * Size, 1, m_pFile);
	}


	template<typename T>
	void Read(T& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}
		if (false == IsRead())
		{
			assert(false);
		}
		fread_s(&_Data, sizeof(T), sizeof(T), 1, m_pFile);
	}


	template<>
	void Read(std::wstring& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}
		if (false == IsRead())
		{
			assert(false);
		}

		int Size;
		fread_s(&Size, sizeof(int), sizeof(int), 1, m_pFile);

		wchar_t* Arr = new wchar_t[Size + 1]{ 0, };

		fread_s(Arr, sizeof(wchar_t)* Size, sizeof(wchar_t) * Size, 1, m_pFile);
		_Data = Arr;

		if (nullptr != Arr)
		{
			delete[] Arr;
			Arr = nullptr;
		}
	}

public:
	bool IsOpen()
	{
		return nullptr != m_pFile;
	}

	bool IsWrite()
	{
		if (false == IsOpen())
		{
			return false;
		}

		if ((size_t)-1 == m_OpenMode.find_first_of(L'w'))
		{
			return false;
		}

		return true;
	}

	bool IsRead()
	{
		if (false == IsOpen())
		{
			return false;
		}

		if ((size_t)-1 == m_OpenMode.find_first_of(L'r'))
		{
			return false;
		}

		return true;

	}

	bool IsExist()
	{
		return GamePath::IsExist(m_Path.c_str());
	}

private:
	void Copy(const GameFile& _Other);

public:
	GameFile& operator=(const GameFile& _Other)
	{
		Close();
		Copy(_Other);
	}

public:
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);
	void PathSetting(const std::wstring& _Path);

	bool Open(const wchar_t* _Path, const wchar_t* _Mode);
	bool Open();

	void Close();

	inline const std::wstring& Ext() const
	{
		return m_Ext;
	}

	inline const std::wstring& CFileName() const
	{
		return m_FileName;
	}

public:
	GameFile();
	GameFile(const GameFile& _Other);
	GameFile(const wchar_t* _Path, const wchar_t* _Mode);
	~GameFile();
};

