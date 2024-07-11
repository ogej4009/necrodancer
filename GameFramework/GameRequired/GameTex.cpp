#include "GameTex.h"
#include "GameMacro.h"
#include "GameFile.h"
#include "GameWin.h"



GameTex::GameTex() 
	: m_Image(nullptr)
	, m_hBitMap(nullptr)
	, m_hOldBitMap(nullptr)
{
}


GameTex::~GameTex()
{
	DeleteObject(m_hOldBitMap);
	DeleteObject(m_Image);
}

bool GameTex::Load(const GameFile& _File)
{
	return Load(_File.CFullPath());
}

bool GameTex::Load(const std::wstring& _Path)
{
	//윈도우 함수 LoadImageW
	//리소스파일에서 LoadStringW를 하던 함수와 동일한 기능이다.
	//인스턴스 핸들, 경로, 이미지 타입, 이미지에서의 시작 위치, 로드타입을 입력받아 이미지를 로드해 온다.
	//그리고 물론 주는 건 핸들이다. 그 핸들은 void*의 기본 HANDLE이고 그걸 HBITMAP 형태로 바꿔서
	//사용할 수 있다.
	m_hBitMap = (HBITMAP)LoadImageW(nullptr, _Path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (nullptr == m_hBitMap)
	{
		ASSERTMSG;
		return false;
	}

	//비어있는 Hdc를 만들어 주는 함수이다.
	//1,1짜리 최소형의 그림 개체가 생성된다.
	m_Image = CreateCompatibleDC(nullptr);

	m_hOldBitMap = (HBITMAP)SelectObject(m_Image, m_hBitMap);

	if (nullptr == m_Image)
	{
		ASSERTMSG;
		return false;
	}

	CalTexSize(_Path);

	return true;

}


void GameTex::CalTexSize(const std::wstring& _Path)
{
	GameFile File = GameFile(_Path.c_str(), L"rb");

	//파일의 시작부분부터 일정한 위치마다 해당 그림파일에 대한 정보가 들어있다.(헤더 위치)
	//해당 헤더를 읽어들이는 방법으로 BITMAPFILEHEADER의 크기만큼 읽어와서 그곳으로
	//값을 입력하게 해주면 손쉽게 사용할 수 있다.
	//파일포인터 위치를 이어서 계속 읽으면 그다음 정보를 읽어올 수 있다.
	File.Read(m_BitFileHeader);
	File.Read(m_BitInfo);

	//m_Siz*e.X = (float)m_BitInfo.biWidth;
	//m_Size.Y = (float)m_BitInfo.biHeight;
}

int GameTex::GetTexPixel(int _X, int _Y)
{
	//GetPixel은 윈도우 함수다.
	return GetPixel(m_Image, _X, _Y);
}

bool GameTex::Create(int _X, int _Y)
{
	//새 HDC 만들기
	//1,1로 먼저 만들고
	m_Image = CreateCompatibleDC(nullptr);
	//기존 HDC와 새로 입력받은 X,Y를 가지고 하나 만듬
	m_hBitMap = CreateCompatibleBitmap(GameWin::MainHDC(), _X, _Y);
	//1,1과 새로 만든 HDC를 변경함
	m_hOldBitMap = (HBITMAP)SelectObject(m_Image, m_hBitMap);

	if (nullptr == m_Image)
	{
		ASSERTMSG;
		return false;
	}

	return true;
}