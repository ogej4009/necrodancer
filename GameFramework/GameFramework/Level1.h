#pragma once
#include "GameState.h"
#include <GameSound.h>


class Level1 : public GameState
{
public:
	Level1();
	~Level1();

private:
	GameSound::GameSoundStream* L1Sound;
	MapEditor* m_Level1;
	CVector::MapIndex m_NextLevel;

public:
	void Loading() override;
	void StateUpdate() override;
	void SoundOn() override;

};

