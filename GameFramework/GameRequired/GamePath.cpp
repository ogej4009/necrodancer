#include "GamePath.h"
#include <atlstr.h>


GamePath::GamePath()
{
}


GamePath::~GamePath()
{
}


void GamePath::PathSetting(const char* _Path)
{
	PathSetting(CA2W(_Path).m_psz);
}

void GamePath::PathSetting(const wchar_t* _Path)
{
	m_Path = _Path;
}