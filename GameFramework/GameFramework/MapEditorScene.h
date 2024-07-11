#pragma once
#include "GameState.h"

class MapEditorScene : public GameState
{
public:
	MapEditorScene();
	~MapEditorScene();

public:
	void Loading() override;
	void StateUpdate() override;
};

