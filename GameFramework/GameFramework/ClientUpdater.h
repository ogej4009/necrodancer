#pragma once
#include <GameWin.h>


class ClientUpdater : public GameWin::LOOPUPDATER
{
public:
	// LOOPUPDATER��(��) ���� ��ӵ�
	void GameInit() override;
	void GameUpdate() override;
	void ResLoad();

public:
	ClientUpdater();
	~ClientUpdater();
};
