#pragma once
#include <GameSound.h>
#include "GameState.h"


class BossRoom : public GameState
{
private:
	GameSound::GameSoundStream* BossRoomSound;
	MapEditor* m_BossRoom;
	CVector::MapIndex m_NextLevel;


public:
	void Loading() override;
	void StateUpdate() override;
	void SoundOn() override;

public:
	BossRoom();
	~BossRoom();
};

