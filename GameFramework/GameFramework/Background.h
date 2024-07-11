#pragma once
#include "GameActor.h"


class Background : public GameActor
{
private:
	GameRender* m_Background;

public:
	Background();
	~Background();
};

