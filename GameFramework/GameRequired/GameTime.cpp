#include "GameTime.h"


GameTime::GameTimer GameTime::m_MainTimer;
float GameTime::m_Second = 0;
int GameTime::m_Minute = 0;
int GameTime::m_Hour = 0;

float GameTime::GameTimer::Update()
{
	QueryPerformanceCounter(&m_CurCPUCount);  //윈도우함수

	m_DDeltaTime = ((double)(m_CurCPUCount.QuadPart - m_PrevCPUCount.QuadPart)) / (double)m_CPUCount.QuadPart;
	m_DeltaTime = (float)m_DDeltaTime;
	//m_CurCPUCount.HighPart LARGE_INTEGER는 이런식으로 4바이트씩 위아래를 나눠놨다.
	//m_CurCPUCount.LowPart  Union 문법 활용함

	m_PrevCPUCount = m_CurCPUCount;
	return m_DeltaTime;
}

void GameTime::GameTimer::Reset()
{ //초기화
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
