#pragma once
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include "GameMacro.h"



//다 랩핑시키고 사용 인터페이스만 호출하게 해줌
//높은 응집도와 낮은 결합도를 지키기 위해서
//응집도 -> 자기 할 일을 혼자 다 할수 있을수록 응집도가 높은것
//결합도 -> 다른 클래스와 관련이 적을수록 낮다고 한다.

class GameInput
{
private:
	class STATICSGAMEINPUT
	{
	public:
		~STATICSGAMEINPUT();
	};
	friend STATICSGAMEINPUT;
	static STATICSGAMEINPUT m_Inst; //static 로딩 기법,싱글톤 대용으로 쓰는듯

    // 포인터형 싱글톤
	//static GAMEINPUT* m_pInst;
	//static GAMEINPUT* Inst()
	//{
	//	if (nullptr == m_pInst)
	//	{
	//		m_pInst = new GAMEINPUT();
	//	}
	//	return m_pInst;
	//}

// 이녀석은 내가 원할때 부술수 있을까? 
/*
	static GAMEINPUT& Inst()
	{
		static GAMEINPUT Inst;
		return Inst;
	}
*/

private:
	// 키 하나가 하나의 개념이니까
	class GameKey
	{ 
		//n개의 int(KeyCode)를 가지고 있어야 한다.
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
	//wchar_t*대신 wstring를 쓰는 이유
	//wchar_t*는 주소만 다르면 내부 내용이 뭐든 상관없이 키값으로 받아들인다.
	static std::map<std::wstring, GameKey* > m_AllKey;
	static GameKey* CurCreateKey;

private:
	//wchar_t* 형태로 상수도 받을 수 있게 되지만 std::wstring으로의 형변환이
	//일어나고 그에따른 비용도 지불해야 한다.
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

