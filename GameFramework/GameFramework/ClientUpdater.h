#pragma once
#include <GameWin.h>


class ClientUpdater : public GameWin::LOOPUPDATER
{
public:
	// LOOPUPDATER을(를) 통해 상속됨
	void GameInit() override;
	void GameUpdate() override;
	void ResLoad();

public:
	ClientUpdater();
	~ClientUpdater();
};
