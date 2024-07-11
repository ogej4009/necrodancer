#include "GameDebug.h"


GameDebug::GameDebug()
{
}

GameDebug::~GameDebug()
{
}

void GameDebug::LeakCheckSetting()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}