#pragma once
#include "GameActor.h"
#include <GameSound.h>


class ShopKeeper : public GameActor
{
private:
	AniRender* m_Keeper;
	bool m_bClosePlayer;
	bool m_bPlayMusic;
	bool m_Start;
	int m_Len;
	bool m_firstContact;
	float m_Time;
	static unsigned int m_KeeperTime;

	GameSound::GameSoundStream* m_KeeperSong;

	enum ShopState
	{
		IDLE,
		ShadowIDLE
	};


public:
	ShopKeeper(CVector _TargetPos);
	~ShopKeeper();

public:
	friend MapEditor;
	void Update() override;
	static unsigned int Time()
	{
		return m_KeeperTime;
	}

private:
	void InitAni(CVector _TargetPos);
	void CheckPlayer();


};

