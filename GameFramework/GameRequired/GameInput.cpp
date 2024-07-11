#include "GameInput.h"
#include "GameTime.h"


std::map<std::wstring, GameInput::GameKey*> GameInput::m_AllKey;
GameInput::GameKey* GameInput::CurCreateKey;

GameInput::STATICSGAMEINPUT GameInput::m_Inst;

GameInput::STATICSGAMEINPUT::~STATICSGAMEINPUT()
{
	std::map<std::wstring, GameInput::GameKey*>::iterator StartIter = GameInput::m_AllKey.begin();
	std::map<std::wstring, GameInput::GameKey*>::iterator EndIter = GameInput::m_AllKey.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	m_AllKey.clear();

} //얘 소멸자가 호출될 때 남아있는 Key들의 메모리를 해제하고 지움


GameInput::GameInput()
{
}


GameInput::~GameInput()
{
}

////////////////////GameKey
//만들어진 키는 이름과 함께 allKey<name,GameKey>에 저장이 되고
//map find 명령을 통해서 해당 키의 이름을 검색, GAMEKEY를 리턴받는다.
//GAMEKEY에는 vector로 입력받은 키 값들을 int로 집어넣고
//해당 int가 눌렸는지 그 벡터를 돌면서 확인한다.
//CreateKey에서 가변인자로 int값을 받기때문에 <int>게 붙여줄수 있고
//그냥 L'S'이 문자열을 int vector에 넣어야 되니까 자동 형변환 암시적으로 될 것이다.
//어차피 문자열을 int로 암시적 형변환 하는건 GetAsyncKeyState가 똑같이 하는 것

GameInput::GameKey::GameKey()
	: m_bUp(false),
	m_bDown(false),
	m_bPress(false),
	m_bRelease(true)  //초기에는 안눌린 상태
{

}

bool GameInput::GameKey::AllKeyCheck()
{
	//해당 벡터의 모든 키가 눌렸다면 true
	//하나라도 안눌리면 false
	//vector에선 size_t를 요구함
	for (size_t i = 0; i < m_KeyVec.size(); ++i)
	{
		if (0 == GetAsyncKeyState(m_KeyVec[i]))
		{
			return false;
		}
	}

	return true;
}

void GameInput::GameKey::Update()
{
	wchar_t buff[100] = { 0, };
	//키를 체크해보니 눌렸다.
	if (true == AllKeyCheck())
	{

		//이전 프레임까지 키가 눌린적이 없었다.
		if (true == m_bRelease)
		{
			//키가 떼어진 한 순간
			m_bUp = false;
			//키가 눌린 한 순간
			m_bDown = true;
			//키가 계속 눌리고 있다.
			m_bPress = true;
			//키가 계속 떼어져 있다.
			m_bRelease = false;
		}
		//이전 프레임까지 눌려있었다.
		else if (false == m_bRelease)
		{
			m_bUp = false;
			m_bDown = false;
			m_bPress = true;
			m_bRelease = false;
		}
	}
	else //키가 안눌렸다, 키를 뗏다 체크 통과 못함
	{
		//이전 프레임까지 눌려있었다.
		if (true == m_bDown || true == m_bPress)
		{
			m_bUp = true;
			m_bDown = false;
			m_bPress = false;
			m_bRelease = true;
		}
		//이전 프레임에서 처음으로 키가 떼어졌다.
		//처음으로 떼어진 상태 이후 또 여기로 진입, 완전 손 뗏다
		else if (true == m_bUp)
		{
			m_bUp = false;
			m_bDown = false;
			m_bPress = false;
			m_bRelease = true;
		}
	}
}


GameInput::GameKey* GameInput::FindKey(const wchar_t* _Name)
{
	std::map<std::wstring, GameKey*>::iterator Iter = m_AllKey.find(_Name);
	if (Iter == m_AllKey.end())
	{
		return nullptr;
	}
	return Iter->second;
}

bool GameInput::IsKey(const wchar_t* _Name)
{
	if (m_AllKey.end() == m_AllKey.find(_Name))
	{
		return false;
	}
	return true;
}
// 매 프레임마다 눌렸는지 확인해주기
// GAMEWIN의 데드타임 BaseLoop 부분에 들어간다.
void GameInput::Update()
{
	std::map<std::wstring, GameInput::GameKey*>::iterator StartIter = GameInput::m_AllKey.begin();
	std::map<std::wstring, GameInput::GameKey*>::iterator EndIter = GameInput::m_AllKey.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			StartIter->second->Update();
		}
	}
}

bool GameInput::Press(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bPress; //상태변화는 update에서, 상태 확인은 각 상태 bool값 리턴으로 
}

bool GameInput::Down(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bDown; //상태변화는 update에서, 상태 확인은 각 상태 bool값 리턴으로 
}

bool GameInput::Up(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bUp; //상태변화는 update에서, 상태 확인은 각 상태 bool값 리턴으로 
}

bool GameInput::Release(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bRelease; //상태변화는 update에서, 상태 확인은 각 상태 bool값 리턴으로 
}
