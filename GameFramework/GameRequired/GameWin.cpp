#include "GameWin.h"
#include "GameInput.h"
#include "GameTime.h"
#include "GameMath.h"
#include "GameSound.h"


std::map<std::wstring, WNDCLASSEXW> GameWin::m_ClassMap; //RegClass 데이터(이름)
std::map<std::wstring, GameWin* > GameWin::m_WinMap;  //GameWin 클래스 데이터(이름)
std::map<HWND, GameWin* > GameWin::m_HWndMap;     //SGMAEWIN 클래스 데이터(윈도우핸들)
GameWin* GameWin::m_pMainWindow = nullptr;
HDC GameWin::m_MainDc;

//WinProc과 같은 함수 포인터, 윈도우 메세지를 처리
LRESULT CALLBACK GameWin::MSGFUNC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroySGameWin(hWnd);

		// 윈도우에 디스트로이 메세지를 발생시킨다.
		// DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int GameWin::BaseLoop(GameWin::LOOPUPDATER& _LoopObj)
{
	GameSound::SoundInit();
	_LoopObj.GameInit();
	MSG msg;
	while (0 != m_WinMap.size())
	{

		//GetMessage는 메세지가 들어올 때까지 계속 대기한다.
		if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameTime::Update();
			//GameSound::SoundUpdate();
			CVector::DIRUPDATE();
			GameInput::Update();
			_LoopObj.GameUpdate();
			//메세지 데드타임
			//게임은 이 함수 안에서 시작해서 이 함수 안에서 끝난다.
		}
	}

	GameSound::SoundEnd();
	return (int)msg.wParam;
}

GameWin::WININIT GameWin::Construct = GameWin::WININIT();

GameWin::WININIT::WININIT()
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GameWin::MSGFUNC;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = 0;
	wcex.hIcon = 0;/*LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPI))*/;
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"DefaultWinClass";
	wcex.hIconSm = 0;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegWindowClass(wcex);
}




GameWin::WININIT::~WININIT() {

}

GameWin::GameWin(HWND _HWnd) : m_HWnd(_HWnd)
{
	m_DC = GetDC(_HWnd); //GetDC 윈도우 함수
}

GameWin::GameWin()
{
}

GameWin::~GameWin()
{
}

void GameWin::DestroySGameWin(HWND _HWnd)
{
	std::map<HWND, GameWin* >::iterator FindIter = m_HWndMap.find(_HWnd);
	if (FindIter == m_HWndMap.end())
	{
		ASSERTMSG;
	}

	std::map<std::wstring, GameWin* >::iterator NameFindIter = m_WinMap.find(FindIter->second->m_Name);;
	/*if (NameFindIter == m_WinMap.end())
	{
		ASSERTMSG;
	}*/

	delete FindIter->second; //GameWin 개체 삭제,생성 윈도우의 종료는 종료 메세지를 호출해야 한다.
	m_WinMap.erase(NameFindIter);
	m_HWndMap.erase(FindIter);

}


bool GameWin::IsRegClass(const wchar_t* _Name)
{
	std::map<std::wstring, WNDCLASSEXW>::iterator FindIter = m_ClassMap.find(_Name);
	if (FindIter == m_ClassMap.end())
	{
		return false;
	}
	return true;
}

void GameWin::RegWindowClass(const WNDCLASSEXW& _wcex)
{
	// 이미 등록된 클래스여도 터트려
	if (true == IsRegClass(_wcex.lpszClassName))
	{
		ASSERTMSG;
	}

	// 등록이 실패해도 폭파.
	if (0 == RegisterClassExW(&_wcex))  //실제 등록 및 검증 
	{
		ASSERTMSG;
	}

	m_ClassMap.insert(std::map<std::wstring, WNDCLASSEXW>::value_type(_wcex.lpszClassName, _wcex));
}

GameWin* GameWin::FindSGameWin(const wchar_t* _WinName)
{
	std::map<std::wstring, GameWin*>::iterator FindIter = m_WinMap.find(_WinName);
	if (FindIter == m_WinMap.end())
	{
		return nullptr;
	}
	return FindIter->second;
}


GameWin* GameWin::CreateSGameWin(const wchar_t* _WinName, const wchar_t* _ClassName /*= L"DefaultWinClass"*/, bool _bShow /*= true*/)
{
	if (nullptr != FindSGameWin(_WinName))
	{
		ASSERTMSG;
	}

	if (false == IsRegClass(_ClassName))
	{
		ASSERTMSG;
	}

	HWND hWnd = CreateWindowW(_ClassName, _WinName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, /*hInstance*/0, nullptr);

	if (0 == hWnd)
	{
		return FALSE;
	}

	GameWin* NewWin = new GameWin(hWnd); //동적생성
	NewWin->m_Style = WS_OVERLAPPEDWINDOW;
	NewWin->m_Name = _WinName;
	m_WinMap.insert(std::map<std::wstring, GameWin*>::value_type(_WinName, NewWin));
	m_HWndMap.insert(std::map<HWND, GameWin*>::value_type(hWnd, NewWin));

	if (1 == m_WinMap.size())
	{
		m_pMainWindow = NewWin;
		m_MainDc = m_pMainWindow->WINHDC(); //GetDc 윈도우 함수로 HDC가져오고 그걸 반환
	}


	if (true == _bShow)
	{
		NewWin->Show();
	}

	return NewWin;
}

void GameWin::Show()
{
	ShowWindow(m_HWnd, SW_SHOW);
}

bool GameWin::Size(CVector _Size)
{
	m_Size = _Size;
	// 크기가 -> 바와 그것까지 합친 사이즈를 800
	// 맴버변수가 모두 PUBLIC이면 요런식으로 생성이 가능.
	RECT Rc = { 0 , 0, m_Size.IX(), m_Size.IY() };
	HMENU MenuCon = GetMenu(m_HWnd); // window 함수 getMenu, 메뉴 핸들을 리턴

	if (nullptr == MenuCon)
	{
		AdjustWindowRect(&Rc, m_Style, FALSE);
	}
	else {
		AdjustWindowRect(&Rc, m_Style, TRUE);
	}

	int x = Rc.right - Rc.left;
	int y = Rc.bottom - Rc.top;

	return SetWindowPos(m_HWnd, 0, 0, 0, Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);

}


bool GameWin::TryReSizeWindow(const int& _Width, const int& _Height)
{
	CVector TempVec = { (float)_Width, (float)_Height };
	return Size(TempVec);
}

//현재 윈도우 기준으로 재정렬한 마우스 위치 리턴
CVector GameWin::GetMousePos(const wchar_t* _WinName)
{
	POINT p = { 0,0 };
	CVector mousePos = { 0,0 };
	if (GetCursorPos(&p))
	{
		GameWin* TEMP = GameWin::FindSGameWin(_WinName);
		ScreenToClient(TEMP->GetHWND(), &p);
		mousePos = { (float)p.x, (float)p.y };
	}
	else
	{
		ASSERTMSG;
	}

	return mousePos;
}