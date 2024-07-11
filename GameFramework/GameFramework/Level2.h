#pragma once
#include "GameState.h"
#include <GameSound.h>


class Level2 : public GameState
{
private:
	GameSound::GameSoundStream* L2Sound;
	MapEditor* m_Level2;
	CVector::MapIndex m_NextLevel;
	bool Check;

public:
	Level2();
	~Level2();

public:
	void Loading() override;
	void StateUpdate() override;
	void SoundOn() override;

};

