#include "ClientUpdater.h"
#include <GameWin.h>
#include <GameDic.h>
#include <GameTexMgr.h>
#include <GameDebug.h> 


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	ClientUpdater ApiUpdater;
	GameDebug::LeakCheckSetting();
	GameWin* NewWindow = GameWin::CreateSGameWin(L"WINDOW");

	float number1 = 3.24242f;
	int number2 = static_cast<int>(number1);
	double num = 0.0f;
	int num2 = static_cast<int>(num);
	//NewWindow->ReSizeWindow(800, 600);
	//SGAMEWIN::MainWindow()->ReSizeWindow(1000, 2000);

	NewWindow->Size(CVector{ 960,540 });
	return GameWin::BaseLoop(ApiUpdater);
}