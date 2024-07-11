#pragma once
#include <GameSound.h>
#include "GameState.h"


class GameActor;
class GameRender;
class Title : public GameState
{
private:
	enum class eTitle
	{
		MainMenu,
		Continue
	};

	GameActor* m_TitleActor;
	GameRender* m_Menu;
	GameRender* m_Continue;
	GameSound::GameSoundStream* TitleSound;

public:
	void Loading() override;
	void StateUpdate() override; 
	void SoundOn() override;

public:
	Title();
	~Title();
};
