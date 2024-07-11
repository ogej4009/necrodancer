#pragma once
#include <Windows.h>


class GameTime
{
	// ���� �ʴ� �����ִ� ���� : 10
	// ����üũ     ����üũ                    ��ŸŸ��
	// 0            0             0 - 0 / 10   = 0.0
	// 5            0             5 - 0 / 10   = 0.5
	// 10           5             10 - 5 / 10  = 0.5
	// 13           10            13 - 10 / 10 = 0.3

private:
	class GameTimer
	{
	public:
		friend GameTime;
	private:
		//Large Integer �� ������.h�� �ִ� �ڷ��� 64��Ʈ ��Ʈ
		LARGE_INTEGER m_CPUCount; // CPU�� 1�ʿ� �� �� �ִ� ����
		LARGE_INTEGER m_CurCPUCount;
		LARGE_INTEGER m_PrevCPUCount;
		double m_DDeltaTime;
		float m_DeltaTime;

	public:
		float Update();
		void Reset();

	private:
		GameTimer();
		~GameTimer();

	};

private: //GameTimer has a
	static GameTimer m_MainTimer;
	static float m_Second;
	static int m_Minute;
	static int m_Hour;

public:
	static float Update()
	{
		return m_MainTimer.Update();
	}

	static float DeltaTime()
	{
		return m_MainTimer.m_DeltaTime;
	}

	static int FPS()
	{
		return  (int)(1.0 / m_MainTimer.m_DeltaTime);
	}

	static int Second()
	{
		m_Second += m_MainTimer.m_DeltaTime;
		int temp = (int)m_Second;
		return temp;
	}

	static void SecondClear()
	{
		m_Second = 0;
	}

	static int Minute()
	{
		if (m_Second > 1 && 0 == (int)m_Second % 60)
		{
			m_Minute += 1;
			m_Second = 0.0f;
		}
		return m_Minute;
	}

	static int Hour()
	{
		if (m_Minute > 1 && 0 == m_Minute % 60)
		{
			m_Hour += 1;
			m_Minute = 0;
		}
		return m_Hour;
	}

public:
	GameTime();
	~GameTime();
};

