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

} //�� �Ҹ��ڰ� ȣ��� �� �����ִ� Key���� �޸𸮸� �����ϰ� ����


GameInput::GameInput()
{
}


GameInput::~GameInput()
{
}

////////////////////GameKey
//������� Ű�� �̸��� �Բ� allKey<name,GameKey>�� ������ �ǰ�
//map find ����� ���ؼ� �ش� Ű�� �̸��� �˻�, GAMEKEY�� ���Ϲ޴´�.
//GAMEKEY���� vector�� �Է¹��� Ű ������ int�� ����ְ�
//�ش� int�� ���ȴ��� �� ���͸� ���鼭 Ȯ���Ѵ�.
//CreateKey���� �������ڷ� int���� �ޱ⶧���� <int>�� �ٿ��ټ� �ְ�
//�׳� L'S'�� ���ڿ��� int vector�� �־�� �Ǵϱ� �ڵ� ����ȯ �Ͻ������� �� ���̴�.
//������ ���ڿ��� int�� �Ͻ��� ����ȯ �ϴ°� GetAsyncKeyState�� �Ȱ��� �ϴ� ��

GameInput::GameKey::GameKey()
	: m_bUp(false),
	m_bDown(false),
	m_bPress(false),
	m_bRelease(true)  //�ʱ⿡�� �ȴ��� ����
{

}

bool GameInput::GameKey::AllKeyCheck()
{
	//�ش� ������ ��� Ű�� ���ȴٸ� true
	//�ϳ��� �ȴ����� false
	//vector���� size_t�� �䱸��
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
	//Ű�� üũ�غ��� ���ȴ�.
	if (true == AllKeyCheck())
	{

		//���� �����ӱ��� Ű�� �������� ������.
		if (true == m_bRelease)
		{
			//Ű�� ������ �� ����
			m_bUp = false;
			//Ű�� ���� �� ����
			m_bDown = true;
			//Ű�� ��� ������ �ִ�.
			m_bPress = true;
			//Ű�� ��� ������ �ִ�.
			m_bRelease = false;
		}
		//���� �����ӱ��� �����־���.
		else if (false == m_bRelease)
		{
			m_bUp = false;
			m_bDown = false;
			m_bPress = true;
			m_bRelease = false;
		}
	}
	else //Ű�� �ȴ��ȴ�, Ű�� �´� üũ ��� ����
	{
		//���� �����ӱ��� �����־���.
		if (true == m_bDown || true == m_bPress)
		{
			m_bUp = true;
			m_bDown = false;
			m_bPress = false;
			m_bRelease = true;
		}
		//���� �����ӿ��� ó������ Ű�� ��������.
		//ó������ ������ ���� ���� �� ����� ����, ���� �� �´�
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
// �� �����Ӹ��� ���ȴ��� Ȯ�����ֱ�
// GAMEWIN�� ����Ÿ�� BaseLoop �κп� ����.
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
	return FKey->m_bPress; //���º�ȭ�� update����, ���� Ȯ���� �� ���� bool�� �������� 
}

bool GameInput::Down(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bDown; //���º�ȭ�� update����, ���� Ȯ���� �� ���� bool�� �������� 
}

bool GameInput::Up(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bUp; //���º�ȭ�� update����, ���� Ȯ���� �� ���� bool�� �������� 
}

bool GameInput::Release(const wchar_t* _Name)
{
	GameKey* FKey = FindKey(_Name);
	if (nullptr == FKey)
	{
		ASSERTMSG;
	}
	return FKey->m_bRelease; //���º�ȭ�� update����, ���� Ȯ���� �� ���� bool�� �������� 
}
