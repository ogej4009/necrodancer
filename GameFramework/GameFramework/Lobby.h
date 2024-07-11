#pragma once
#include "GameState.h"
#include <GameSound.h>

class MapEditor;
class GameActor;
class Lobby : public GameState
{
public:
	Lobby();
	~Lobby();

private:
	GameSound::GameSoundStream* LobbySound;
	MapEditor* m_LobbyMap;
	CVector::MapIndex m_NextLevel;

public:
	void Loading() override;
	void StateUpdate() override;
	void SoundOn() override;
};

