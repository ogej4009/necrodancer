#pragma once
#include "GameActor.h"
#include <GameSound.h>


class MapEditor;
class GameDoor : public GameActor
{
private:
	bool m_bIsSide;
	GameRender * m_Door;
	bool m_bOpen;
	float m_DelayTime;

public:
	GameDoor(CVector _TargetPos, bool _bIsSide);
	~GameDoor();

public:
	friend MapEditor;

public:
	void Update() override;
	void OpenDoor();

};

