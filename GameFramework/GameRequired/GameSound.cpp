#include "GameSound.h"
#include "GameMacro.h"
#include "GameDic.h"
#include <atlstr.h>


#ifdef RELEASEMODE
#ifdef X32
#pragma comment(lib, "fmod_vc.lib")
#else X64
#pragma comment(lib, "fmod_vc.lib")
#endif
#else DEBUGMODE
#ifdef X32
#pragma comment(lib, "fmodL_vc.lib")
#else X64
#pragma comment(lib, "fmodL_vc.lib")
#endif
#endif


FMOD::System* GameSound::m_pSoundSystem;
std::map<std::wstring, GameSound*> GameSound::m_AllSound;
std::list<GameSound::GameSoundStream*> GameSound::m_AllStream;
std::list<GameSound::GameSoundStream*> GameSound::m_ActiveStream;


void GameSound::GameSoundStream::Init(FMOD::Channel* _Ch)
{
	m_bActive = true;
	m_Ch = _Ch;
}


void GameSound::GameSoundStream::Stop()
{
	m_Ch->stop();
	m_bActive = false;

	// 값을 넣어줘서 안에서 검색이 일어나게 되서.
	// 일반적인 함수보다는 느릴것이다.
	m_ActiveStream.remove(this);
	m_AllStream.push_back(this);
}


void GameSound::GameSoundStream::Position(unsigned int _TimeSecond)
{
	// milisecond 1000 = 1sec
	m_Ch->setPosition(_TimeSecond * 1000, FMOD_TIMEUNIT_MS);

}

unsigned int GameSound::GameSoundStream::Position()
{
	unsigned int temp = 0;
	// milisecond 1000 = 1sec
	m_Ch->getPosition(&temp, FMOD_TIMEUNIT_MS);
	return temp;

}

void GameSound::GameSoundStream::SoundVolume(float _Volume)
{
	m_Ch->setVolume(_Volume);
}

void GameSound::GameSoundStream::ReleaseStream()
{

}

void GameSound::GameSoundStream::Paused()
{
	m_Ch->setPaused(true);
}

void GameSound::GameSoundStream::ReStart()
{
	m_Ch->setPaused(false);
}

void GameSound::GameSoundStream::PlayOnOff()
{
	bool Check;

	m_Ch->getPaused(&Check);
	if (false == Check)
	{
		Paused();
	}
	else {
		ReStart();
	}
}

GameSound::GameSoundStream* GameSound::PopSoundStream()
{


	if (0 == m_AllStream.size())
	{
		return nullptr;
	}

	GameSound::GameSoundStream* pHGSS = m_AllStream.front();
	m_AllStream.pop_front();
	return pHGSS;
}

GameSound::GameSoundStream::GameSoundStream() : m_bActive(false)
{
}

GameSound::GameSoundStream::~GameSoundStream()
{
}



GameSound::GameSound()
{
}


GameSound::~GameSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
	}
}


void GameSound::SoundInit(int count /*= 1024*/)
{
	FMOD::System_Create(&m_pSoundSystem);

	// 사운드 드라이버와 연결되었다.
	if (nullptr == m_pSoundSystem)
	{
		ASSERTMSG;
	}

	if (FMOD_OK != m_pSoundSystem->init(32, FMOD_DEFAULT, nullptr))
	{
		ASSERTMSG;
	}

	
	for (int i = 0; i < count; i++)
	{
		m_AllStream.push_back(new GameSoundStream());
	}

}


void GameSound::SoundEnd() {

	while (0 < m_AllSound.size())
	{
		if (nullptr != m_AllSound.begin()->second)
		{
			delete m_AllSound.begin()->second;
			m_AllSound.erase(m_AllSound.begin());
		}
	}

	for (GameSoundStream* HSS : m_AllStream)
	{
		delete HSS;
	}

	for (GameSoundStream* HSS : m_ActiveStream)
	{
		delete HSS;
	}

}

void GameSound::SoundUpdate()
{
	m_pSoundSystem->update();
}
void GameSound::SoundDestroy()
{
}


void GameSound::SoundLoad(const GameFile& _File)
{
	if (m_AllSound.end() != m_AllSound.find(_File.CFileName()))
	{
		ASSERTMSG;
		return;
	}

	GameSound* NewSound = new GameSound();
	if (false == NewSound->Load(_File.CFullPath().c_str()))
	{
		ASSERTMSG;
	}
	m_AllSound.insert(std::map<std::wstring, GameSound*>::value_type(_File.CFileName(), NewSound));
}



GameSound::GameSoundStream* GameSound::SoundPlay(const wchar_t* _Name, int _Loop)
{
	std::map<std::wstring, GameSound*>::iterator FindSoundIter = m_AllSound.find(_Name);

	if (m_AllSound.end() == FindSoundIter)
	{
		ASSERTMSG;
		return nullptr;
	}

	GameSoundStream* pStream = PopSoundStream();

	if (nullptr == pStream)
	{
		return nullptr;
	}

	// 재생스트림을 넘겨준다.
	// 진행중인 어떠어떠한 것.
	// CH->setLoopCount(_Loop);
	m_pSoundSystem->playSound(FindSoundIter->second->m_pSound, nullptr, false, &pStream->m_Ch);
	FindSoundIter->second->m_pSound->setLoopCount(_Loop);

	if (_Loop == 0)
	{
		ASSERTMSG;
	}

	if (1 != _Loop)
	{
		pStream->m_Ch->setMode(FMOD_LOOP_NORMAL);
		pStream->m_Ch->setLoopCount(_Loop);
	}

	m_ActiveStream.push_back(pStream);

	return pStream;
	// CH->setLoopPoints()

	// CH->getPosition 현재 어디까지 재생됐냐.
	// CH->getPitch 속력
}

void GameSound::SoundPlayToOne(const wchar_t* _Name)
{
	std::map<std::wstring, GameSound*>::iterator FindSoundIter = m_AllSound.find(_Name);

	if (m_AllSound.end() == FindSoundIter)
	{
		ASSERTMSG;
		return;
	}

	// 재생스트림을 넘겨준다.
	// 진행중인 어떠어떠한 것.
	// CH->setLoopCount(_Loop);
	m_pSoundSystem->playSound(FindSoundIter->second->m_pSound, nullptr, false, nullptr);

	return;
	// CH->setLoopPoints()

	// CH->getPosition 현재 어디까지 재생됐냐.
	// CH->getPitch 속력
}


bool GameSound::Load(const wchar_t* _Path)
{
	// m_pSound->getLength()

	if (FMOD_OK != m_pSoundSystem->createSound(CW2A(_Path, CP_UTF8).m_psz, FMOD_DEFAULT, nullptr, &m_pSound))
	{
		ASSERTMSG;
		return false;
	}

	return true;
}