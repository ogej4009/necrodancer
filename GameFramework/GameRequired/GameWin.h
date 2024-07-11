#pragma once
#include <windows.h>
#include <map>
#include <string>
#include "GameMath.h"
#include "GameMacro.h"


class GameWin
{
	//////////추상클래스
public:
	class LOOPUPDATER
	{
	public:
		virtual void GameUpdate() = 0;
		virtual void GameInit() = 0;
	};

////Regist before Create Class
private:
	class WININIT
	{
	public:
		WININIT();
		~WININIT();
	};

	static WININIT Construct;

/////////////////static 멤버 변수,함수들
private:
	static std::map<std::wstring, WNDCLASSEXW> m_ClassMap;  // 클래스 이름으로 클래스 개체 찾기
	static std::map<std::wstring, GameWin* > m_WinMap; //윈도우 이름으로 개체 찾기
	static std::map<HWND, GameWin* > m_HWndMap; //윈도우 핸들로 클래스 개체 찾기
	static GameWin* m_pMainWindow;
	static HDC m_MainDc;

public: //get set
	static GameWin* MainWindow()
	{
		return m_pMainWindow;
	}

	static HDC MainHDC()
	{
		return m_MainDc;
	}

private:
	static void DestroySGameWin(HWND _HWnd);

public:
	static LRESULT __stdcall MSGFUNC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static bool IsRegClass(const wchar_t* _ClassName);
	static void RegWindowClass(const WNDCLASSEXW& _wcex);
	static GameWin* FindSGameWin(const wchar_t* _WinName);
	static GameWin* CreateSGameWin(const wchar_t* _WinName, const wchar_t* _ClassName = L"DefaultWinClass", bool _bShow = true);
	static int BaseLoop(LOOPUPDATER& _LoopObj);
	static CVector GetMousePos(const wchar_t* _WinName);

public:
	bool TryReSizeWindow(const int& _width,const int& _height);
	
	CVector MousePos()
	{
		POINT Point;

		GetCursorPos(&Point);
		ScreenToClient(m_HWnd, &Point);

		CVector ReturnPos;

		ReturnPos.X = (float)Point.x;
		ReturnPos.Y = (float)Point.y;

		return ReturnPos;
	}

private: ////member
	std::wstring m_Name;
	int m_Style;
	HWND m_HWnd; // 윈도우 간섭 권한
	HDC m_DC; //윈도우 그릴수 있는 권한
	CVector m_Size;

public:
	HDC WINHDC() //멤버 HDC리턴
	{
		return m_DC;
	}

	HWND GetHWND()
	{
		return m_HWnd;
	}


public:
	void Show();
	bool Size(CVector _Size);
	CVector Size() { return m_Size; }

private:
	GameWin(HWND _HWnd);
	GameWin();

private:
	virtual ~GameWin();
};

