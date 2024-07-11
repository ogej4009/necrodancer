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
	//������ �Լ� LoadImageW
	//���ҽ����Ͽ��� LoadStringW�� �ϴ� �Լ��� ������ ����̴�.
	//�ν��Ͻ� �ڵ�, ���, �̹��� Ÿ��, �̹��������� ���� ��ġ, �ε�Ÿ���� �Է¹޾� �̹����� �ε��� �´�.
	//�׸��� ���� �ִ� �� �ڵ��̴�. �� �ڵ��� void*�� �⺻ HANDLE�̰� �װ� HBITMAP ���·� �ٲ㼭
	//����� �� �ִ�.
	m_hBitMap = (HBITMAP)LoadImageW(nullptr, _Path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (nullptr == m_hBitMap)
	{
		ASSERTMSG;
		return false;
	}

	//����ִ� Hdc�� ����� �ִ� �Լ��̴�.
	//1,1¥�� �ּ����� �׸� ��ü�� �����ȴ�.
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

	//������ ���ۺκк��� ������ ��ġ���� �ش� �׸����Ͽ� ���� ������ ����ִ�.(��� ��ġ)
	//�ش� ����� �о���̴� ������� BITMAPFILEHEADER�� ũ�⸸ŭ �о�ͼ� �װ�����
	//���� �Է��ϰ� ���ָ� �ս��� ����� �� �ִ�.
	//���������� ��ġ�� �̾ ��� ������ �״��� ������ �о�� �� �ִ�.
	File.Read(m_BitFileHeader);
	File.Read(m_BitInfo);

	//m_Siz*e.X = (float)m_BitInfo.biWidth;
	//m_Size.Y = (float)m_BitInfo.biHeight;
}

int GameTex::GetTexPixel(int _X, int _Y)
{
	//GetPixel�� ������ �Լ���.
	return GetPixel(m_Image, _X, _Y);
}

bool GameTex::Create(int _X, int _Y)
{
	//�� HDC �����
	//1,1�� ���� �����
	m_Image = CreateCompatibleDC(nullptr);
	//���� HDC�� ���� �Է¹��� X,Y�� ������ �ϳ� ����
	m_hBitMap = CreateCompatibleBitmap(GameWin::MainHDC(), _X, _Y);
	//1,1�� ���� ���� HDC�� ������
	m_hOldBitMap = (HBITMAP)SelectObject(m_Image, m_hBitMap);

	if (nullptr == m_Image)
	{
		ASSERTMSG;
		return false;
	}

	return true;
}