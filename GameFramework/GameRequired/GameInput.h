#pragma once
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include "GameMacro.h"



//�� ���ν�Ű�� ��� �������̽��� ȣ���ϰ� ����
//���� �������� ���� ���յ��� ��Ű�� ���ؼ�
//������ -> �ڱ� �� ���� ȥ�� �� �Ҽ� �������� �������� ������
//���յ� -> �ٸ� Ŭ������ ������ �������� ���ٰ� �Ѵ�.

class GameInput
{
private:
	class STATICSGAMEINPUT
	{
	public:
		~STATICSGAMEINPUT();
	};
	friend STATICSGAMEINPUT;
	static STATICSGAMEINPUT m_Inst; //static �ε� ���,�̱��� ������� ���µ�

    // �������� �̱���
	//static GAMEINPUT* m_pInst;
	//static GAMEINPUT* Inst()
	//{
	//	if (nullptr == m_pInst)
	//	{
	//		m_pInst = new GAMEINPUT();
	//	}
	//	return m_pInst;
	//}

// �̳༮�� ���� ���Ҷ� �μ��� ������? 
/*
	static GAMEINPUT& Inst()
	{
		static GAMEINPUT Inst;
		return Inst;
	}
*/

private:
	// Ű �ϳ��� �ϳ��� �����̴ϱ�
	class GameKey
	{ 
		//n���� int(KeyCode)�� ������ �־�� �Ѵ�.
		friend GameInput;

	private:
		bool m_bUp;
		bool m_bDown;
		bool m_bPress;
		bool m_bRelease;
		
		std::vector<int> m_KeyVec;

	public:
		void Update();
		bool AllKeyCheck();

	public:
		void PushKey(int _Value)
		{
			m_KeyVec.push_back(_Value);
		}

	public:
		GameKey();
		~GameKey() {}
	};

private:
	//wchar_t*��� wstring�� ���� ����
	//wchar_t*�� �ּҸ� �ٸ��� ���� ������ ���� ������� Ű������ �޾Ƶ��δ�.
	static std::map<std::wstring, GameKey* > m_AllKey;
	static GameKey* CurCreateKey;

private:
	//wchar_t* ���·� ����� ���� �� �ְ� ������ std::wstring������ ����ȯ��
	//�Ͼ�� �׿����� ��뵵 �����ؾ� �Ѵ�.
	static GameKey* FindKey(const wchar_t* _Name);
	static bool IsKey(const wchar_t* _Name);

public:
	static void Update();

public:

	template<typename ...REST>
	static bool CreateKey(const wchar_t* _Name, REST... _Value)
	{
		if (true == IsKey(_Name))
		{
			ASSERTMSG;
			return false;
		}
		CurCreateKey = new GameKey();
		PushKey(_Value ...);
		m_AllKey.insert(std::map<std::wstring, GameKey*>::value_type(_Name, CurCreateKey));
		return true;
	}

private:
	template<typename ...REST>
	static void PushKey(int _PushValue, REST... _Value)
	{
		CurCreateKey->m_KeyVec.push_back(_PushValue);
		PushKey(_Value...);
	}

	static void PushKey()
	{
	}

public:
	static bool Press(const wchar_t* _Name);
	static bool Down(const wchar_t* _Name);
	static bool Up(const wchar_t* _Name);
	static bool Release(const wchar_t* _Name);

public:
	GameInput();
	~GameInput();
};

