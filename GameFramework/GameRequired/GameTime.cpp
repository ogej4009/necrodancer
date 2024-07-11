#include "GameTime.h"


GameTime::GameTimer GameTime::m_MainTimer;
float GameTime::m_Second = 0;
int GameTime::m_Minute = 0;
int GameTime::m_Hour = 0;

float GameTime::GameTimer::Update()
{
	QueryPerformanceCounter(&m_CurCPUCount);  //�������Լ�

	m_DDeltaTime = ((double)(m_CurCPUCount.QuadPart - m_PrevCPUCount.QuadPart)) / (double)m_CPUCount.QuadPart;
	m_DeltaTime = (float)m_DDeltaTime;
	//m_CurCPUCount.HighPart LARGE_INTEGER�� �̷������� 4����Ʈ�� ���Ʒ��� ��������.
	//m_CurCPUCount.LowPart  Union ���� Ȱ����

	m_PrevCPUCount = m_CurCPUCount;
	return m_DeltaTime;
}

void GameTime::GameTimer::Reset()
{ //�ʱ�ȭ
	QueryPerformanceFrequency(&m_CPUCount);
	QueryPerformanceCounter(&m_CurCPUCount);
	QueryPerformanceCounter(&m_PrevCPUCount);
}

GameTime::GameTimer::GameTimer()
{
	Reset();
}


GameTime::GameTimer::~GameTimer()
{

}


GameTime::GameTime()
{
	
}


GameTime::~GameTime()
{
}
