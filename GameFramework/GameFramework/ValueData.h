#pragma once

//CreateCol에서 League값을 넣어줄때 집어넣는다.
enum class eColGroup
{
	PLAYER,
	MONSTER,
	PLAYERBULLET,
	MONSTERBULLET,
	MOUSE,
	UI,
};

class ValueData
{
public:
	ValueData() {}
	~ValueData() {}
};
